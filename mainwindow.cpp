#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newform.h"
#include <QPainter>
#include <QPicture>
#include <avLabel.h>
#include <QVector>
#include <QMenu>
#include <QPushButton>
#include <QCheckBox>
#include "entertext.h"
#include "insets.h"
#include "actions.h"
#include <QTextStream>



struct cmms{
    QavLabel    *comps;
    int         ipx;        // координаты в сетке
    int         ipy;
    int         type;       // тип компонента
    int         cX;         // растяжение ячеек по X
    int         cY;         // ... Y
    QString     name;       // имя компонента
    QList <int> actions;      // действия (события)
    QList <QString> methods;  // методы к действиям
    QList <QStringList> codes;// коды методов
};

struct jcomps{
    QavLabel   *comps;
};

int buf=-1;         // буфер программы для копирования компонентов (индекс из cms)
newForm *nf;
QavLabel *label_4;  // окно проекта
int gH, gW;         // размер ячеек сетки
int mvx, mvy;       // временные переменные для перемещения comp
bool inMove=false;  // флаг режима перемещения
QRect rc;           // рект места базирования comp
//std::vector <cmms*> cms;  // массив компонентов в сетке
QList <cmms*> cms;
int cmsCou=0;       // количество компонентов в сетке
int selComp;        // индекс кликнутого компонента
int cX, cY;         // коэф. увеличения по  Х и У при замещении более чем 1 ячейки сетки
bool isCreate=false;// класс формы создан
int compAll=6;      // всего доступно компонентов
QMenu       *menu;
QMenu       *fillMenu;
QMenu       *anchorMenu;
QMenu       *typeMenu;
QMenu       *sizeMenu;
QMenu       *menuWindow;
enterText   *et;
Insets      *inst;
Actions     *acts;
QString     formName="?";
//std::vector <int> imports;      // вектор список импортируемых компонентов
std::vector <jcomps> comp;      // вектор заготовок компонентов
QavLabel    *resizer;
QavLabel    *titler;
QStringList events;             // импорты различных events
QStringList jComp;              // импорты компонентов
QPoint      tPn;                // temp

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label_4=new QavLabel(ui->label);
    //label_4->setGeometry(30,57,291,271);
    label_4->setText("New window. Create new class.");
    label_4->setFrameStyle(QFrame::Panel);
    label_4->setStyleSheet("background-color: rgb(243, 239, 239);");
    connect(label_4,SIGNAL(rclicked(int,int,int)),this,SLOT(openMenuWindow(int,int,int)));
    jComponents();
    setMenu();
    setResizer();
    setMenuWindow();

}

QStringList MainWindow::addLines(QStringList sl, QStringList l){
    bool f=true;
    for(int i=0;i<l.size();i++){
        f=true;
        for(int j=0;j<sl.size();j++){
            if(sl[j]==l[i]){
                f=false;
                break;
            }
        }
        if(f)sl.append(l[i]);
    }
    return sl;
}

void MainWindow::setResizer(){ // создать регулятоp размера окна
    //label_4->setParent(ui->label);
    label_4->setGeometry(5,25,300,300);
    resizer = new QavLabel(ui->label);
    resizer->setPixmap(QPixmap(":/new/prefix1/resizer"));
    resizer->setScaledContents(true);
    resizer->setGeometry(label_4->geometry().left()+label_4->geometry().width()-8,
                         label_4->geometry().top()+label_4->geometry().height()-8,
                         16,16);
    connect(resizer,SIGNAL(mouse_press(int,int,int)),this,SLOT(resizerPress(int,int,int)));
    connect(resizer,SIGNAL(mouse_move(int,int,int)),this,SLOT(resizerMove(int,int,int)));
    connect(resizer,SIGNAL(mouse_up(int,int,int)),this,SLOT(resizerUp(int,int,int)));
    // перемещение заготовки окна целиком
    titler = new QavLabel(ui->label);
    titler->setGeometry(5,5,300,20);
    titler->setStyleSheet("background-color: rgb(0, 0, 242);color: rgb(241, 241, 25)");
    titler->setText("Title window");
    connect(titler,SIGNAL(mouse_press(int,int,int)),this,SLOT(resizerPress(int,int,int)));
    connect(titler,SIGNAL(mouse_move(int,int,int)),this,SLOT(titleMove(int,int,int)));
    connect(titler,SIGNAL(mouse_up(int,int,int)),this,SLOT(resizerUp(int,int,int)));
}


void MainWindow::titleMove(int x, int y, int i){  // перемещение заготовки окна
    if(inMove){
        int dx=x-mvx;
        int dy=y-mvy;
        mvx=x;
        mvy=y;
        label_4->setGeometry(label_4->geometry().left()+dx,
                                 label_4->geometry().top()+dy,
                                 label_4->geometry().width(),
                                 label_4->geometry().height());
        titler->setGeometry(titler->geometry().left()+dx,
                                 titler->geometry().top()+dy,
                                 titler->geometry().width(),
                                 titler->geometry().height());
        resizer->setGeometry(resizer->geometry().left()+dx,
                             resizer->geometry().top()+dy,
                             resizer->geometry().width(),
                             resizer->geometry().height());
    }
}

void MainWindow::resizerPress(int x, int y, int i){  // нажатие на resizer
    mvx=x;
    mvy=y;
    inMove=true;
}

void MainWindow::resizerMove(int x, int y, int i){  // перемещение resizer'a
    if(inMove){
        int dx=x-mvx;
        int dy=y-mvy;
        int w=resizer->geometry().width();
        int h=resizer->geometry().height();
        int xx=resizer->geometry().left();
        int yy=resizer->geometry().top();
        resizer->setGeometry(xx+dx,yy+dy,w,h);
        mvx=x;
        mvy=y;
        label_4->setGeometry(label_4->geometry().left(),
                                 label_4->geometry().top(),
                                 xx+8-label_4->geometry().left(),
                                 yy+8-label_4->geometry().top());
        titler->setGeometry(titler->geometry().left(),
                                 titler->geometry().top(),
                                 label_4->geometry().width(),
                                 titler->geometry().height());
        drawGrid();
    }
}

void MainWindow::resizerUp(int x, int y, int i){ // отжатие
    inMove=false;
}

void MainWindow::jComponents(){
    for(int i=0;i<compAll;i++){
        comp.push_back(jcomps());
        comp[i].comps=new QavLabel(this);
        comp[i].comps->setPixmap(QPixmap(getPictureName(i)));
        comp[i].comps->setScaledContents(true);
        comp[i].comps->setImnum(i);
    }
}

QString MainWindow::getPictureName(int index){
    switch (index) {
    case 0:
        return ":/new/prefix1/button";
        break;
    case 1:
        return ":/new/prefix1/checkbox";
        break;
    case 2:
        return ":/new/prefix1/label";
        break;
    case 3:
        return ":/new/prefix1/textfield";
        break;
    case 4:
        return ":/new/prefix1/textarea";
        break;
    case 5:
        return ":/new/prefix1/combobox";
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMenuWindow(){
    menuWindow = new QMenu("",this);
    QAction *paste = new QAction ("Paste", menuWindow);
    connect(paste, SIGNAL(triggered()), this, SLOT(pasteComponent()));
    menuWindow->addAction(paste);
}

void MainWindow::setMenu(){     // построить меню настройки компонента
    menu = new QMenu("Settings",this);
    fillMenu = new QMenu("Filling cells (fill)",this);
    anchorMenu = new QMenu("Alignment component (anchor)",this);
    typeMenu = new QMenu("Select type of component", this);
    sizeMenu = new QMenu("Resize", this);

    QAction *setName = new QAction ("Set Name", menu);
    connect(setName, SIGNAL(triggered()), this, SLOT(setNameComp()));
    menu->addAction(setName);

    QAction *setJButton = new QAction ("JButton", typeMenu);
    connect(setJButton, SIGNAL(triggered()), this, SLOT(setTypeJButton()));
    typeMenu->addAction(setJButton);
    QAction *setJCheckBox = new QAction ("JCheckBox", typeMenu);
    connect(setJCheckBox, SIGNAL(triggered()), this, SLOT(setTypeJCheckbox()));
    typeMenu->addAction(setJCheckBox);
    QAction *setJComboBox = new QAction ("JComboBox", typeMenu);
    connect(setJComboBox, SIGNAL(triggered()), this, SLOT(setTypeJComboBox()));
    typeMenu->addAction(setJComboBox);
    QAction *setJLabel = new QAction ("JLabel", typeMenu);
    connect(setJLabel, SIGNAL(triggered()), this, SLOT(setTypeJLabel()));
    typeMenu->addAction(setJLabel);
    menu->addMenu(typeMenu);
    QAction *setJTextField = new QAction ("JTextField", typeMenu);
    connect(setJTextField, SIGNAL(triggered()), this, SLOT(setTypeJTextField()));
    typeMenu->addAction(setJTextField);
    QAction *setJTextArea = new QAction ("JTextArea", typeMenu);
    connect(setJTextArea, SIGNAL(triggered()), this, SLOT(setTypeJTextArea()));
    typeMenu->addAction(setJTextArea);
    menu->addMenu(typeMenu);

    QAction *setText = new QAction ("Set text (if posible)", menu);
    connect(setText, SIGNAL(triggered()), this, SLOT(setTextComp()));
    menu->addAction(setText);

    QAction *add_Act = new QAction ("Add action", menu);
    connect(add_Act, SIGNAL(triggered()), this, SLOT(addMethod()));
    menu->addAction(add_Act);

    menu->addSeparator();

    QAction *horpls = new QAction ("Increase horizontal", sizeMenu);
    connect(horpls, SIGNAL(triggered()), this, SLOT(incrHor()));
    sizeMenu->addAction(horpls);
    QAction *horpms = new QAction ("Decrease horizontal", sizeMenu);
    connect(horpms, SIGNAL(triggered()), this, SLOT(decrHor()));
    sizeMenu->addAction(horpms);
    QAction *verpls = new QAction ("Increase vertical", sizeMenu);
    connect(verpls, SIGNAL(triggered()), this, SLOT(incrVer()));
    sizeMenu->addAction(verpls);
    QAction *verpms = new QAction ("Decrease vertical", sizeMenu);
    connect(verpms, SIGNAL(triggered()), this, SLOT(decrVer()));
    sizeMenu->addAction(verpms);
    menu->addMenu(sizeMenu);

    menu->addSeparator();

    QAction *fillNONE = new QAction ("NONE", fillMenu);
    connect(fillNONE, SIGNAL(triggered()), this, SLOT(setFillNone()));
    fillMenu->addAction(fillNONE);
    QAction *fillBOTH = new QAction ("BOTH", fillMenu);
    connect(fillBOTH, SIGNAL(triggered()), this, SLOT(setFillBoth()));
    fillMenu->addAction(fillBOTH);
    menu->addMenu(fillMenu);
    QAction *fillHORIZONTAL = new QAction ("HORIZONTAL", fillMenu);
    connect(fillHORIZONTAL, SIGNAL(triggered()), this, SLOT(setFillHor()));
    fillMenu->addAction(fillHORIZONTAL);
    QAction *fillVERTICAL = new QAction ("VERTICAL", fillMenu);
    connect(fillVERTICAL, SIGNAL(triggered()), this, SLOT(setFillVer()));
    fillMenu->addAction(fillVERTICAL);
    menu->addMenu(fillMenu);

    QAction *anchorCENTER = new QAction ("CENTER", anchorMenu);
    connect(anchorCENTER, SIGNAL(triggered()), this, SLOT(setAnchorCen()));
    anchorMenu->addAction(anchorCENTER);
    QAction *anchorNORTH = new QAction ("NORTH", anchorMenu);
    connect(anchorNORTH, SIGNAL(triggered()), this, SLOT(setAnchorNorth()));
    anchorMenu->addAction(anchorNORTH);
    QAction *anchorSOUTH = new QAction ("SOUTH", anchorMenu);
    connect(anchorSOUTH, SIGNAL(triggered()), this, SLOT(setAnchorSouth()));
    anchorMenu->addAction(anchorSOUTH);
    QAction *anchorEAST = new QAction ("EAST", anchorMenu);
    connect(anchorEAST, SIGNAL(triggered()), this, SLOT(setAnchorEast()));
    anchorMenu->addAction(anchorEAST);
    QAction *anchorWEST = new QAction ("WEST", anchorMenu);
    connect(anchorWEST, SIGNAL(triggered()), this, SLOT(setAnchorWest()));
    anchorMenu->addAction(anchorWEST);
    QAction *anchorNORHEAST = new QAction ("NORHEAST", anchorMenu);
    connect(anchorNORHEAST, SIGNAL(triggered()), this, SLOT(setAnchorNorthEast()));
    anchorMenu->addAction(anchorNORHEAST);
    QAction *anchorNORTHWEST = new QAction ("NORTHWEST", anchorMenu);
    connect(anchorNORTHWEST, SIGNAL(triggered()), this, SLOT(setAnchorNorthWest()));
    anchorMenu->addAction(anchorNORTHWEST);
    QAction *anchorSOUTHEAST = new QAction ("SOUTHEAST", anchorMenu);
    connect(anchorSOUTHEAST, SIGNAL(triggered()), this, SLOT(setAnchorSouthEast()));
    anchorMenu->addAction(anchorSOUTHEAST);
    QAction *anchorSOUTHWEST = new QAction ("SOUTHWEST", anchorMenu);
    connect(anchorSOUTHWEST, SIGNAL(triggered()), this, SLOT(setAnchorSouthWest()));
    anchorMenu->addAction(anchorSOUTHWEST);
    menu->addMenu(anchorMenu);

    QAction *weightXY = new QAction ("Weight of component (weightx/weighty)", menu);
    connect(weightXY, SIGNAL(triggered()), this, SLOT(setSizeComp()));
    //menu->addAction(weightXY);
    
    QAction *ipadXY = new QAction ("Increase of component (ipadx/ipady)", menu);
    connect(ipadXY, SIGNAL(triggered()), this, SLOT(setSizeComp()));
    menu->addAction(ipadXY);
    
    QAction *insets = new QAction ("Indents from the edges (insets)", menu);
    connect(insets, SIGNAL(triggered()), this, SLOT(setInsets()));
    menu->addAction(insets);

    menu->addSeparator();
    QAction *delcomp = new QAction ("Delete", menu);
    connect(delcomp, SIGNAL(triggered()), this, SLOT(delComponent()));
    menu->addAction(delcomp);
    QAction *copycomp = new QAction ("Copy", menu);
    connect(copycomp, SIGNAL(triggered()), this, SLOT(copyComponent()));
    menu->addAction(copycomp);


}

void MainWindow::setSizeComp(){

}

void MainWindow::copyComponent(){
    buf=selComp;
}

void MainWindow::openMenuWindow(int x, int y, int index){
        tPn.setX(x);
        tPn.setY(y);
        menuWindow->exec(QCursor::pos());
}

void MainWindow::pasteComponent(){
    if(buf>-1){
        QPoint pn;
        pn.setX(((tPn.x()-label_4->geometry().x()-ui->label->geometry().x()-this->geometry().x())/gW)*gW);
        pn.setY(((tPn.y()-label_4->geometry().y()-ui->label->geometry().y()-this->geometry().y())/gH)*gH);
        QRect rc(cms[buf]->comps->geometry());
        cms.push_back(new cmms());
        cms[cmsCou]->cX=cms[buf]->cX;
        cms[cmsCou]->cY=cms[buf]->cY;
        cms[cmsCou]->name="comp_"+QString::number(cmsCou);    // присвоить стандартное имя
        cms[cmsCou]->comps = new QavLabel(label_4);
        cms[cmsCou]->comps->setGeometry(pn.x()+1,pn.y(),rc.width(),rc.height());
        cms[cmsCou]->comps->setPixmap(QPixmap(":/new/prefix1/jbutton"));
        cms[cmsCou]->comps->setImnum(cmsCou);
        cms[cmsCou]->comps->setComponent(cms[buf]->comps->getComponent());
        cms[cmsCou]->comps->setTextComponent(cms[buf]->comps->getTextComponent());
        connect(cms[cmsCou]->comps,SIGNAL(mouse_press(int,int,int)),this,SLOT(cmsPress(int,int,int)));
        connect(cms[cmsCou]->comps,SIGNAL(mouse_move(int,int,int)),this,SLOT(cmsMove(int,int,int)));
        connect(cms[cmsCou]->comps,SIGNAL(mouse_up(int,int,int)),this,SLOT(cmsUp(int,int,int)));
        cms[cmsCou]->comps->setEnabledComponent(ui->checkBox_2->isChecked());
        cms[cmsCou]->comps->show();
        connect(cms[cmsCou]->comps,SIGNAL(rclicked(int,int,int)),this,SLOT(cmsRClick(int,int,int)));
        cms[cmsCou]->ipx=cms[buf]->ipx;
        cms[cmsCou]->ipy=cms[buf]->ipy;
        cms[cmsCou]->comps->setFill(cms[buf]->comps->getFill());
        cms[cmsCou]->comps->setAnchor(cms[buf]->comps->getAnchor());
        cms[cmsCou]->comps->setInsets(cms[buf]->comps->getInsets());
        cms[cmsCou++]->comps->setScaledContents(true);
    }

}

void MainWindow::delComponent(){
    for(int i=selComp+1;i<cms.size();i++){
        cms[i]->comps->setImnum(i-1);
    }
    cms[selComp]->comps->deleteLater();
    cms.removeAt(selComp);
}

void MainWindow::addMethod(){
    if(acts==0){
        acts=new Actions();
        connect(acts, SIGNAL(setMethod(int,QString,QStringList)),
                this, SLOT(addMethod_end(int,QString,QStringList)));
        connect(acts, SIGNAL(getMethod(int)),this,SLOT(loadMethod(int)));
        connect(acts, SIGNAL(delMethod(int)),this,SLOT(delMethod(int)));
    }
    acts->setType(cms[selComp]->comps->getComponent(), cms[selComp]->name);
    acts->show();
}

void MainWindow::delMethod(int index){
    clearIfNeed(index);
}

void MainWindow::addMethod_end(int act, QString nm, QStringList sl){
    clearIfNeed(act);
    cms[selComp]->actions.push_back(act);
    cms[selComp]->methods.push_back(nm);
    cms[selComp]->codes.push_back(sl);

}

void MainWindow::clearIfNeed(int act){
    for(int i=0;i<cms[selComp]->actions.size();i++){
        if(cms[selComp]->actions[i]==act){
            cms[selComp]->codes.removeAt(i);
            cms[selComp]->methods.removeAt(i);
            cms[selComp]->actions.removeAt(i);
            }
            return;
        }
}

void MainWindow::loadMethod(int index){
    for(int i=0;i<cms[selComp]->actions.size();i++){
        if(cms[selComp]->actions[i]==index){
            acts->loadMethod(index,cms[selComp]->methods[i],cms[selComp]->codes[i]);
            return;
        }
    }
    acts->loadMethod(index,"#",QStringList(""));
}

void MainWindow::setInsets(){
    if(inst==0){
        inst = new Insets();
        connect(inst,SIGNAL(setInsets(QRect)),this,SLOT(setInsets_end(QRect)));
    }
    QRect r=cms[selComp]->comps->getInsets();
    inst->setValue(r);
    inst->show();
}

void MainWindow::setInsets_end(QRect r){
    cms[selComp]->comps->setInsets(r);
}

void MainWindow::setFillNone(){
    cms[selComp]->comps->setFill(0);
}

void MainWindow::setFillBoth(){
    cms[selComp]->comps->setFill(1);
}

void MainWindow::setFillHor(){
    cms[selComp]->comps->setFill(2);
}

void MainWindow::setFillVer(){
    cms[selComp]->comps->setFill(3);
}

void MainWindow::setAnchorCen(){
    cms[selComp]->comps->setAnchor(0);
}

void MainWindow::setAnchorNorth(){
    cms[selComp]->comps->setAnchor(1);
}

void MainWindow::setAnchorSouth(){
    cms[selComp]->comps->setAnchor(2);
}

void MainWindow::setAnchorEast(){
    cms[selComp]->comps->setAnchor(3);
}

void MainWindow::setAnchorWest(){
    cms[selComp]->comps->setAnchor(4);
}

void MainWindow::setAnchorNorthEast(){
    cms[selComp]->comps->setAnchor(5);
}

void MainWindow::setAnchorNorthWest(){
    cms[selComp]->comps->setAnchor(6);
}

void MainWindow::setAnchorSouthEast(){
    cms[selComp]->comps->setAnchor(7);
}

void MainWindow::setAnchorSouthWest(){
    cms[selComp]->comps->setAnchor(8);
}

void MainWindow::incrHor(){
    QRect rc = cms[selComp]->comps->geometry();
    rc = QRect(rc.x(),rc.y(),rc.width()+gW,rc.height());
    cms[selComp]->comps->setGeometry(rc);
    cms[selComp]->cX++;
}

void MainWindow::decrHor(){
    if(cms[selComp]->cX>1){
        QRect rc = cms[selComp]->comps->geometry();
        rc = QRect(rc.x(),rc.y(),rc.width()-gW,rc.height());
        cms[selComp]->comps->setGeometry(rc);
        cms[selComp]->cX--;
    }
}

void MainWindow::incrVer(){
    QRect rc = cms[selComp]->comps->geometry();
    rc = QRect(rc.x(),rc.y(),rc.width(),rc.height()+gH);
    cms[selComp]->comps->setGeometry(rc);
    cms[selComp]->cY++;
}

void MainWindow::decrVer(){
    if(cms[selComp]->cY>1){
        QRect rc = cms[selComp]->comps->geometry();
        rc = QRect(rc.x(),rc.y(),rc.width(),rc.height()-gH);
        cms[selComp]->comps->setGeometry(rc);
        cms[selComp]->cY--;
    }
}

void MainWindow::setTextComp(){ // задать текст компонента
    if(et==0){
        et = new enterText();
        connect(et,SIGNAL(okPress(QString, int)),this,SLOT(setTextComp_end(QString, int)));
    }
    et->setLabel("Enter the text of component");
    //et->clearText();
    et->setText(cms[selComp]->comps->getTextComponent());
    et->types=0; // text
    et->show();
}

void MainWindow::setNameComp(){ // задать имя компонента
    if(et==0){
        et = new enterText();
        connect(et,SIGNAL(okPress(QString, int)),this,SLOT(setTextComp_end(QString, int)));
    }
    et->setLabel("Enter the name of component");
    //et->clearText();
    et->setText(cms[selComp]->name);
    et->types=1; // name
    et->show();
}

void MainWindow::setTextComp_end(QString s, int t){
    switch (t) {
    case 0:
        cms[selComp]->comps->setTextComponent(s);
        break;
    case 1:
        cms[selComp]->name=s;
        break;
    default:
        break;
    }

}

void MainWindow::setTypeJButton(){
    setTypeComp(0);
}

void MainWindow::setTypeJCheckbox(){
    setTypeComp(1);
}

void MainWindow::setTypeJLabel(){
    setTypeComp(2);
}

void MainWindow::setTypeJTextField(){
    setTypeComp(3);
}

void MainWindow::setTypeJTextArea(){
    setTypeComp(4);
}

void MainWindow::setTypeJComboBox(){
    setTypeComp(5);
}

void MainWindow::setTypeComp(int index){
      cms[selComp]->comps->setComponent(index);
}

void MainWindow::createform(int X, int Y, QString s){
    // clear if need
    formName=s;
    titler->setText(s);
    buf=-1;
    ui->label->setText("");
    gW=label_4->geometry().width()/X;
    gH=label_4->geometry().height()/Y;
    drawGrid();
    for(int i=0;i<6;i++){
        connect(comp[i].comps,SIGNAL(mouse_press(int,int,int)),this,SLOT(compPress(int,int,int)));
        connect(comp[i].comps,SIGNAL(mouse_move(int,int,int)),this,SLOT(compMove(int,int,int)));
        connect(comp[i].comps,SIGNAL(mouse_up(int,int,int)),this,SLOT(compUp(int,int,int)));
    }
    isCreate=true;
}

void MainWindow::drawGrid(){
    QPicture pict;
    QPainter painter;
    painter.begin(&pict);
    for(int i=gW;i<label_4->geometry().width();i+=gW){
        painter.drawLine(i,0,i,label_4->geometry().height()); // Рисуем линию
    }
    for(int i=gH;i<label_4->geometry().height();i+=gH){
        painter.drawLine(0,i,label_4->geometry().width(),i); // Рисуем линию
    }
    painter.end();
    label_4->setPicture(pict);
}

void MainWindow::compPress(int x, int y, int i){  // нажатие на comp
    mvx=x;
    mvy=y;
    rc=comp[i].comps->geometry();
    inMove=true;
}

void MainWindow::compMove(int x, int y, int i){  // перемещение
    if(inMove){
        int dx=x-mvx;
        int dy=y-mvy;
        int w=comp[i].comps->geometry().width();
        int h=comp[i].comps->geometry().height();
        int xx=comp[i].comps->geometry().left();
        int yy=comp[i].comps->geometry().top();
        comp[i].comps->setGeometry(xx+dx,yy+dy,w,h);
        mvx=x;
        mvy=y;
    }
}

void MainWindow::compUp(int x, int y, int i){ // отжатие ->> создать новый компонент
    inMove=false;
    QPoint pn=getGridPosition(comp[i].comps, true);
    if(pn.x()<label_4->width() && pn.y()<label_4->height()){
        cms.push_back(new cmms());
        cms[cmsCou]->comps=new QavLabel();
        cms[cmsCou]->cX=1;
        cms[cmsCou]->cY=1;
        cms[cmsCou]->name="comp_"+QString::number(cmsCou);    // присвоить стандартное имя
        cms[cmsCou]->comps = new QavLabel(label_4);
        cms[cmsCou]->comps->setGeometry(pn.x()+1,pn.y(),gW,gH);
        cms[cmsCou]->comps->setPixmap(QPixmap(":/new/prefix1/jbutton"));
        cms[cmsCou]->comps->setImnum(cmsCou);
        cms[cmsCou]->comps->setComponent(i);
        connect(cms[cmsCou]->comps,SIGNAL(mouse_press(int,int,int)),this,SLOT(cmsPress(int,int,int)));
        connect(cms[cmsCou]->comps,SIGNAL(mouse_move(int,int,int)),this,SLOT(cmsMove(int,int,int)));
        connect(cms[cmsCou]->comps,SIGNAL(mouse_up(int,int,int)),this,SLOT(cmsUp(int,int,int)));
        cms[cmsCou]->comps->setEnabledComponent(ui->checkBox_2->isChecked());
        cms[cmsCou]->comps->show();
        connect(cms[cmsCou]->comps,SIGNAL(rclicked(int,int,int)),this,SLOT(cmsRClick(int,int,int)));
        cms[cmsCou]->ipx=(pn.x()+1)/gW;
        cms[cmsCou]->ipy=(pn.y()+1)/gH;
        cms[cmsCou++]->comps->setScaledContents(true);
        //addJComp(getImportComp(i));
    }
    comp[i].comps->setGeometry(rc);
}

QStringList MainWindow::getImportComp(int i){
    QStringList sl;
    sl.append("import javax.swing."+getTextType(i)+";");
    if(i==4)sl.append("import javax.swing.BorderFactory;");
    return sl;
}

void MainWindow::cmsPress(int x, int y, int i){  // нажатие на cms[]
    mvx=x;
    mvy=y;
    cX=cms[i]->cX;
    cY=cms[i]->cY;
    inMove=true;
}

void MainWindow::cmsMove(int x, int y, int i){  // перемещение cms[]
    if(inMove){
        int dx=x-mvx;
        int dy=y-mvy;
        int w=cms[i]->comps->geometry().width();
        int h=cms[i]->comps->geometry().height();
        int xx=cms[i]->comps->geometry().left();
        int yy=cms[i]->comps->geometry().top();
        cms[i]->comps->setGeometry(xx+dx,yy+dy,w,h);
        mvx=x;
        mvy=y;
    }
}

void MainWindow::cmsUp(int x, int y, int i){ // отжатие cms[]
    inMove=false;
    QPoint pn=getGridPosition(cms[i]->comps, false);
    cms[i]->comps->setGeometry(pn.x()+1,pn.y(),
                             cms[i]->comps->geometry().width(),
                              cms[i]->comps->geometry().height());
    cms[i]->ipx=(pn.x()+1)/gW;
    cms[i]->ipy=(pn.y()+1)/gH;
}

void MainWindow::cmsRClick(int x, int y, int i){
        selComp=i;
        menu->exec(QCursor::pos());
}

QPoint MainWindow::getGridPosition(QavLabel *avl,bool b){
    int selX, selY;
    if(b){ // если это первичная установка
        selX=((avl->geometry().center().x()-label_4->geometry().x()-ui->label->geometry().x())/gW)*gW;
        selY=((avl->geometry().center().y()-label_4->geometry().y()-ui->label->geometry().y())/gH)*gH;
    }else{ // существующий компонент
        selX=((avl->geometry().left()+(gW/2))/gW)*gW;
        selY=((avl->geometry().top()+(gH/2))/gH)*gH;
    }
    return QPoint(selX,selY);
}

void MainWindow::on_pushButton_clicked() // создать новую форму
{
    if(nf==0){
            nf = new newForm();
            connect(nf, SIGNAL(createNewForm(int,int,QString)),this,SLOT(createform(int,int,QString)));
    }
    nf->show();
}

QStringList MainWindow::getImports(int index){ // index = type*10+method
    QStringList sl;
    switch (index) {
    case 31: //
        sl.append("import javax.swing.event.DocumentEvent;");
        sl.append("import javax.swing.event.DocumentListener;");
        break;
    case 40:
        sl.append("import java.awt.event.KeyEvent;");
        sl.append("import java.awt.event.KeyListener;");
        break;
    default:
        sl.append("import java.awt.event.ActionEvent;");
        sl.append("import java.awt.event.ActionListener;");
        break;
    }
    return sl;
}

void MainWindow::on_pushButton_2_clicked()  // создать код
{
    QStringList form;
    form.append("// Auto-generated form with GBLayout");
    form.append("        ");
    form.append("import java.awt.GridBagConstraints;");
    form.append("import java.awt.GridBagLayout;");
    form.append("import java.awt.Insets;");
    form.append("import javax.swing.JFrame;");
    jComp.clear();
    for(int i=0;i<cms.size();i++){  // импорт компонентов
        jComp=addLines(jComp,getImportComp(cms[i]->comps->getComponent()));
    }
    form.append(jComp);
    events.clear();
    for(int i=0;i<cms.size();i++){  // импорт событий и слушателей, если необходимо
        for(int j=0;j<cms[i]->actions.size();j++){
            events=addLines(events,getImports(cms[i]->comps->getComponent()*10+cms[i]->actions[j]));
        }
    }
    form.append(events);
    form.append("        ");
    form.append("public class "+ formName +" extends JFrame{");
    for(int i=0;i<cms.size();i++){
        form.append("    "+getTextType(cms[i]->comps->getComponent())+" "+cms[i]->name+";");
    }
    form.append("        ");
    form.append("    public "+formName+"(){");
    form.append("        // TODO Auto-generated method stub");
    form.append("        super(\""+formName+"\");  // Еnter here your window title");
    form.append("        setBounds(300, 300, 300, 400);");
    form.append("        create_form();        // Build a form according your project ");
    form.append("    }");
    form.append("    ");
    form.append("    private void create_form(){");
    form.append("        // TODO Auto-generated method stub");
    form.append("        GridBagLayout gbl = new GridBagLayout();");
    form.append("        GridBagConstraints c = new GridBagConstraints();");
    form.append("        setLayout(gbl);");
    for(int i=0; i<cms.size(); i++){
        if(cms[i]->comps->getComponent()==5){
        form.append("        String[] combo"+QString::number(i)+"Strings = { \""+
                    cms[i]->comps->getTextComponent()+ "\" }; // add items here if need");
        form.append("        "+cms[i]->name+" = new "+getTextType(cms[i]->comps->getComponent())+"(combo"+
                            QString::number(i)+"Strings);");
        }
        else{
        form.append("        "+cms[i]->name+" = new "+getTextType(cms[i]->comps->getComponent())+"(\""+
                                cms[i]->comps->getTextComponent()+"\");");
        }
        form.append("        c.fill   = GridBagConstraints."+getTextFill(cms[i]->comps->getFill())+";");
        form.append("        c.anchor  = GridBagConstraints."+getTextAnchor(cms[i]->comps->getAnchor())+";");
        rc=cms[i]->comps->getInsets();
        form.append("        c.insets = new Insets("+QString::number(rc.y())+","+
                                                     QString::number(rc.x())+","+
                                                     QString::number(rc.height())+","+
                                                     QString::number(rc.width())+");");
        form.append("        c.gridheight ="+QString::number(cms[i]->cY)+";");
        form.append("        c.gridwidth  ="+QString::number(cms[i]->cX)+";");
        form.append("        c.gridx = "+QString::number(cms[i]->ipx)+";");
        form.append("        c.gridy = "+QString::number(cms[i]->ipy)+";");
        form.append("        c.ipadx = 0;");
        form.append("        c.ipady = 0;");
        form.append("        c.weightx = 1.0;");
        form.append("        c.weighty = 1.0;");
        if(cms[i]->comps->getComponent()==4)form.append("        "+cms[i]->name+
                                                        ".setBorder(BorderFactory.createTitledBorder(\"\"));");
        form.append("        gbl.setConstraints("+cms[i]->name+", c);");
        form.append("        add("+cms[i]->name+");");

    }
    for(int i=0;i<cms.size();i++){  // подключение методов слушателей
        for(int m=0;m<cms[i]->actions.size();m++){
            form.append(getListener(i, m));// получить код слушателя для этого компонентa и события
        }
     }

    form.append("        ");

    form.append("    }");
    form.append("    ");
    if(ui->checkBox->isChecked()){
        form.append("    public static void main(String[] args) { // This is only necessary if this form of the whole project starts");
        form.append("        "+ formName+" app = new "+formName+"();");
        form.append("        app.setVisible(true);");
        form.append("    }");
        form.append("    ");
    }

    for(int i=0;i<cms.size();i++){  // подключение методов обработки событий
            for(int s=0;s<cms[i]->codes.size();s++)
                for(int j=0;j<cms[i]->codes[s].size();j++){
                        form.append("    "+cms[i]->codes[s][j]);
                    }
        }


    form.append("}");
    // write data
    QFile fOut("file.java");
    if (fOut.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream s(&fOut);
        for (int i=0;i<form.size();++i)
          s << form.at(i) << '\n';
      }
      fOut.close();
}

QStringList MainWindow::getListener(int index, int name){
    switch (cms[index]->comps->getComponent()) {
    case 0: // button
        return getButtonsListener(index, name);
        break;
    case 1: // checkBox
        return getButtonsListener(index, name);
    case 3: // textField
        return getTextFieldListener(index, name);
    case 4: // textArea
        return getTextAreaListener(index, name);
    case 5: // comboBox
        return getComboBoxListener(index, name);
    default:
        break;
    }
}

QStringList MainWindow::getButtonsListener(int index, int name){ // and checkBox
    QStringList sl;
    switch (cms[index]->actions[name]) {
    case 0: // click
        sl.append("        "+cms[index]->name+".addActionListener(new ActionListener() {" );
        sl.append("            @Override");
        sl.append("            public void actionPerformed(ActionEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        sl.append("                "+cms[index]->methods[name]+";");
        sl.append("            }");
        sl.append("          });");
        return sl;
        break;
    default:
        return sl;
        break;
    }
}

QStringList MainWindow::getComboBoxListener(int index, int name){
    QStringList sl;
    switch (cms[index]->actions[name]) {
    case 0: // select item
        sl.append("        "+cms[index]->name+".addActionListener(new ActionListener() {" );
        sl.append("            @Override");
        sl.append("            public void actionPerformed(ActionEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        sl.append("                "+getClearName(cms[index]->methods[name])+"("+cms[index]->name+".getSelectedIndex());");
        sl.append("            }");
        sl.append("          });");
        return sl;
        break;
    default:
        return sl;
        break;
    }
}

QStringList MainWindow::getTextAreaListener(int index, int name){ // and checkBox
    QStringList sl;
    switch (cms[index]->actions[name]) {
    case 0: // keyPress
        sl.append("        "+cms[index]->name+".addKeyListener(new KeyListener() {");
        sl.append("            @Override");
        sl.append("            public void keyTyped(KeyEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        sl.append("            }");
        sl.append("            @Override");
        sl.append("            public void keyReleased(KeyEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        sl.append("            }");
        sl.append("            @Override");
        sl.append("            public void keyPressed(KeyEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        sl.append("                "+getClearName(cms[index]->methods[name])+"(arg0.getExtendedKeyCode());");
        sl.append("            }");
        sl.append("        });");
        return sl;
        break;
    default:
        return sl;
        break;
    }
}

QString MainWindow::getClearName(QString fn){
    return fn.mid(0,fn.indexOf("("));
}

QStringList MainWindow::getTextFieldListener(int index, int name){
    QStringList sl;
    switch (cms[index]->actions[name]) {
    case 0: // onEnter
        sl.append("        "+cms[index]->name+".addActionListener(new ActionListener() {" );
        sl.append("            @Override");
        sl.append("            public void actionPerformed(ActionEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        sl.append("                "+cms[index]->methods[name]+";");
        sl.append("            }");
        sl.append("          });");
        return sl;
        break;
    case 1: // onEditText
        sl.append("        "+cms[index]->name+".getDocument().addDocumentListener(new DocumentListener() {");
        sl.append("            @Override");
        sl.append("            public void changedUpdate(DocumentEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        //sl.append("                "+cms[index]->methods[name]+";");
        sl.append("            }");
        sl.append("            @Override");
        sl.append("            public void removeUpdate(DocumentEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        //sl.append("                "+cms[index]->methods[name]+";");
        sl.append("            }");
        sl.append("            @Override");
        sl.append("            public void insertUpdate(DocumentEvent arg0) {");
        sl.append("            // TODO Auto-generated method stub");
        sl.append("                "+cms[index]->methods[name]+";");
        sl.append("            }");
        sl.append("          });");
    default:
        return sl;
        break;
    }
}


QString MainWindow::getTextFill(int index){
    switch (index) {
    case 0:
        return "NONE";
        break;
    case 1:
        return "BOTH";
        break;
    case 2:
        return "HORIZONTAL";
        break;
    case 3:
        return "VERTICAL";
        break;
    default:
        return "NONE";
        break;
    }
}

QString MainWindow::getTextAnchor(int index){
    switch (index) {
    case 0:
        return "CENTER";
        break;
    case 1:
        return "NORTH";
        break;
    case 2:
        return "SOUTH";
        break;
    case 3:
        return "EAST";
        break;
    case 4:
        return "WEST";
        break;
    case 5:
        return "NORHEAST";
        break;
    case 6:
        return "NORTHWEST";
        break;
    case 7:
        return "SOUTHEAST";
        break;
    case 8:
        return "SOUTHWEST";
        break;
    default:
        return "CENTER";
        break;
    }
}

QString MainWindow::getTextType(int index){
    switch (index) {
    case 0:
        return "JButton";
        break;
    case 1:
        return "JCheckBox";
        break;
    case 2:
        return "JLabel";
        break;
    case 3:
        return "JTextField";
        break;
    case 4:
        return "JTextArea";
        break;
    case 5:
        return "JComboBox";
        break;
    default:
        return "";
        break;
    }
}


void MainWindow::resizeEvent(QResizeEvent *){
    comp[0].comps->setGeometry(this->width()-90,50,80,28);
    comp[1].comps->setGeometry(this->width()-90,80,80,28);
    comp[2].comps->setGeometry(this->width()-90,110,80,28);
    comp[3].comps->setGeometry(this->width()-90,140,80,28);
    comp[4].comps->setGeometry(this->width()-90,170,80,28);
    comp[5].comps->setGeometry(this->width()-90,200,80,28);
    ui->label_2->setGeometry(this->width()-141,20,131,30);
    ui->checkBox->setGeometry(10, this->height()-30,150,30);
    ui->checkBox_2->setGeometry(190, this->height()-30,190,30);
    ui->label->setGeometry(20,30,this->width()-212,this->height()-80);
    resizeGrid();
}

void MainWindow::resizeGrid(){
    if(isCreate){
        drawGrid();
    }
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    for(int i=0;i<cms.size();i++){
        cms[i]->comps->setEnabledComponent(checked);
    }
}
