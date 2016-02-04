#include "avLabel.h"
#include <QPushButton>
#include <QCheckBox>
//#include <qavbutton.h>
//#include <Qt>

//using namespace Qt;

QPixmap *mem_pix;
int bt=0;
QPoint pnt;

QRect       rcComp; // рект компонента
int dx,dy;          // отступы для рисования размещения

QavLabel::QavLabel(QWidget *parent) : QLabel(parent)
{
    mem_pix=new QPixmap(1,1);
    Jfill=0;
    Janchor=6;
    Jinsets=QRect(0,0,0,0);
    curComp=-1;
    pnt=QPoint(0,0);
}

QavLabel::~QavLabel(){}

void QavLabel::mouseReleaseEvent(QMouseEvent *e)
{

        if(e->button() == Qt::LeftButton)
        {
            emit mouse_up(e->globalX(), e->globalY(), this->getImnum());             // левый клик
        }

        if(e->button() == Qt::RightButton)
        {
            if(pnt!=QPoint(e->globalX(),e->globalY())){
                emit rclicked(e->globalX(), e->globalY(), this->getImnum());             // правый клик
                pnt=QPoint(e->globalX(),e->globalY());
            }
        }
    bt=0;
}

void QavLabel::mouseRelease(QMouseEvent *e){
    mouseReleaseEvent(e);
}

void QavLabel::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit mouse_press(e->globalX(), e->globalY(), this->getImnum());               // нажатие
        bt=1;
    }
}

void QavLabel::mousePress(QMouseEvent *e){
    mousePressEvent(e);
}


void QavLabel::mouseMoveEvent(QMouseEvent *e)
{
    if(bt==1)
    {
        emit mouse_move(e->globalX(), e->globalY(), this->getImnum());                    // движение
    }
}

void QavLabel::mouseMove(QMouseEvent *e){
    mouseMoveEvent(e);
}

void QavLabel::wheelEvent(QWheelEvent *event)  // вращение колеса (1) - вверх (-1) - вниз
{
    int i=event->delta();
    if(i>0)
    {
        i=1;
    }
    else
    {
        i=-1;
    }
    emit mouse_wheel(i, Imnum);
}

void QavLabel::wheelEv(QWheelEvent *e){
    wheelEvent(e);
}

void QavLabel::setComponent(int index){
    clearComp();
    curComp=index;
    switch (index) {
    case 0:
        //pb = new QPushButton(this);
        pb = new QavButton(this);
        pb->setGeometry(0,0,this->size().width(),this->size().height());
        connect(pb,SIGNAL(mouse_Release(QMouseEvent*)),this,SLOT(mouseRelease(QMouseEvent*)));
        connect(pb,SIGNAL(mouse_Press(QMouseEvent*)),this,SLOT(mousePress(QMouseEvent*)));
        connect(pb,SIGNAL(mouse_Move(QMouseEvent*)),this,SLOT(mouseMove(QMouseEvent*)));
        pb->show();
        break;
    case 1:
        //chb = new QCheckBox(this);
        chb = new QavCheckbox(this);
        chb->setGeometry(0,0,this->size().width(),this->size().height());
        connect(chb,SIGNAL(mouse_Release(QMouseEvent*)),this,SLOT(mouseRelease(QMouseEvent*)));
        connect(chb,SIGNAL(mouse_Press(QMouseEvent*)),this,SLOT(mousePress(QMouseEvent*)));
        connect(chb,SIGNAL(mouse_Move(QMouseEvent*)),this,SLOT(mouseMove(QMouseEvent*)));
        chb->show();
        break;
    case 2:
        lb = new QLabel(this);
        lb->setGeometry(0,0,this->size().width(),this->size().height());
        lb->show();
        break;
    case 3:
        le = new QLineEdit(this);
        le->setGeometry(0,0,this->size().width(),this->size().height());
        le->show();
        break;
    case 4:
        te = new QTextEdit(this);
        te->setGeometry(0,0,this->size().width(),this->size().height());
        te->show();
        break;
    case 5:
        cb = new QComboBox(this);
        cb->setGeometry(0,0,this->size().width(),this->size().height());
        cb->show();
        break;
    default:
        curComp=-1;
        break;
    }
    setTextComponent(textComp);
}

void QavLabel::clearComp(){
    switch (curComp) {
    case 0:
        pb->deleteLater(); // ->setVisible(false);
        break;
    case 1:
        chb->setVisible(false);
        break;
    case 2:
        lb->setVisible(false);
        break;
    case 3:
        le->setVisible(false);
        break;
    case 4:
        te->setVisible(false);
        break;
    case 5:
        cb->setVisible(false);
        break;
    default:
        break;
    }
}

void QavLabel::setEnabledComponent(bool en){
    switch (curComp) {
    case 0:
        pb->setEnabled(en);
        break;
    case 1:
        chb->setEnabled(en);
        break;
    case 2:
        lb->setEnabled(en);
        break;
    case 3:
        le->setEnabled(en);
        break;
    case 4:
        te->setEnabled(en);
        break;
    case 5:
        cb->setEnabled(en);
        break;
    default:
        break;
    }
}

void QavLabel::setTextComponent(QString s){
    textComp=s;
    switch (curComp) {
    case 0:
        pb->setText(textComp);
        break;
    case 1:
        chb->setText(textComp);
        break;
    case 2:
        lb->setText(textComp);
        break;
    case 3:
        le->setText(textComp);
        break;
    case 4:
        te->setText(textComp);
        break;
    case 5:
        cb->addItem(textComp);
        cb->setCurrentIndex(0);
        cb->setCurrentText(textComp);
        break;
    default:
        break;
    }
    setFill(Jfill);
}

void QavLabel::setFill(int index){
    Jfill=index;
    switch (curComp) {
    case 0:
        pb->adjustSize(); // NONE
        rcComp=pb->geometry();
        selectFill(Jfill);
        setDxDy();
        selectFill(Jfill);
        pb->setGeometry(rcComp);
        break;
    case 1:
        chb->adjustSize();
        rcComp=chb->geometry();
        selectFill(Jfill);
        setDxDy();
        selectFill(Jfill);
        chb->setGeometry(rcComp);
        break;
    case 2:
        lb->adjustSize();
        rcComp=lb->geometry();
        selectFill(Jfill);
        setDxDy();
        selectFill(Jfill);
        lb->setGeometry(rcComp);
        break;
    case 3:
        le->adjustSize();
        rcComp=le->geometry();
        selectFill(Jfill);
        setDxDy();
        selectFill(Jfill);
        le->setGeometry(rcComp);
        break;
    case 4:
        te->adjustSize();
        rcComp=te->geometry();
        selectFill(Jfill);
        setDxDy();
        selectFill(Jfill);
        te->setGeometry(rcComp);
        break;
    case 5:
        cb->adjustSize();
        rcComp=cb->geometry();
        selectFill(Jfill);
        setDxDy();
        selectFill(Jfill);
        cb->setGeometry(rcComp);
        break;
    default:
        break;
    }

}

void QavLabel::selectFill(int i){
    switch (i) {
    case 0:
        rcComp=QRect(dx,dy,rcComp.width(),rcComp.height());
        break;
    case 1:
        rcComp=QRect(dx,dy,this->width()-Jinsets.x()-Jinsets.width(),this->height()-Jinsets.y()-Jinsets.height());     // BOTH
        break;
    case 2:
        rcComp=QRect(dx,dy,this->width()-Jinsets.x()-Jinsets.width(),rcComp.height());    // HORIZONTAL
        break;
    case 3:
        rcComp=QRect(dx,dy,rcComp.width(),this->height()-Jinsets.y()-Jinsets.height());    // VERTICAL
        break;
    default:
        break;
    }
}

void QavLabel::setAnchor(int index){
    Janchor=index;
    setFill(Jfill);
}

void QavLabel::setDxDy(){
    switch (Janchor) {
    case 0: // center
        dx=Jinsets.x()+(this->width()-Jinsets.x()-Jinsets.width()-rcComp.width())/2;
        dy=Jinsets.y()+(this->height()-Jinsets.y()-Jinsets.height()-rcComp.height())/2;
        break;
    case 1: // north
        dx=Jinsets.x()+(this->width()-Jinsets.x()-Jinsets.width()-rcComp.width())/2;
        dy=Jinsets.y();
        break;
    case 2: // south
        dx=Jinsets.x()+(this->width()-Jinsets.x()-Jinsets.width()-rcComp.width())/2;
        dy=Jinsets.y()+(this->height()-Jinsets.y()-Jinsets.height()-rcComp.height());
        break;
    case 3: // East
        dx=Jinsets.x()+(this->width()-Jinsets.x()-Jinsets.width()-rcComp.width());
        dy=Jinsets.y()+(this->height()-Jinsets.y()-Jinsets.height()-rcComp.height())/2;
        break;
    case 4: // west
        dx=Jinsets.x();
        dy=Jinsets.y()+(this->height()-Jinsets.y()-Jinsets.height()-rcComp.height())/2;
        break;
    case 5: // NorhtEast
        dx=Jinsets.x()+(this->width()-Jinsets.x()-Jinsets.width()-rcComp.width());
        dy=Jinsets.y();
        break;
    case 6: // NorthWest
        dx=Jinsets.x();
        dy=Jinsets.y();
        break;
    case 7: // SouthEast
        dx=Jinsets.x()+(this->width()-Jinsets.x()-Jinsets.width()-rcComp.width());
        dy=Jinsets.y()+(this->height()-Jinsets.y()-Jinsets.height()-rcComp.height());
        break;
    case 8: // SouthWest
        dx=Jinsets.x();
        dy=Jinsets.y()+(this->height()-Jinsets.y()-Jinsets.height()-rcComp.height());
        break;
    default:
        break;
    }
}

void QavLabel::setInsets(QRect ins){
    Jinsets=ins;
    setFill(Jfill);
}
