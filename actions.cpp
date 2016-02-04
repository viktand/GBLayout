#include "actions.h"
#include "ui_actions.h"
#include <QMessageBox>

QString Name;
int Type;
bool isLoad=true;

Actions::Actions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Actions)
{
    ui->setupUi(this);
    setEnabledButtons(false);
}

void Actions::setEnabledButtons(bool b){
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(b);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(b);
}

void Actions::on_buttonBox_clicked(QAbstractButton *button){
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok){
        emit setMethod(ui->comboBox->currentIndex(), ui->label_4->text(), ui->textEdit->toPlainText().split("\n"));
    }
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply){
        emit setMethod(ui->comboBox->currentIndex(), ui->label_4->text(), ui->textEdit->toPlainText().split("\n"));
        ui->checkBox->setChecked(true);
        setEnabledButtons(false);
    }
}

Actions::~Actions()
{
    delete ui;
}


void Actions::setType(int type, QString name){
    isLoad=true;
    ui->textEdit->clear();
    Name=name;
    Type=type;
    loadActions();
    isLoad=false;
    ui->textEdit->setFocus();
}

void Actions::loadActions(){
    isLoad=true;
    switch (Type) {
    case 0:
        loadJButton();
        break;
    case 1:
        loadJCheckBox();
        break;
    case 2:
        // label
        break;
    case 3:
        loadJTextField();
        break;
    case 4:
        loadJTextArea();
        break;
    case 5:
        loadJComboBox();
        break;
    default:
        break;
    }
    isLoad=false;
}

void Actions::loadJButton(){
    ui->comboBox->clear();
    ui->comboBox->addItem("onClick");
    // добавить другие варианты здесь
    emit getMethod(0);
}

void Actions::loadJCheckBox(){
    ui->comboBox->clear();
    ui->comboBox->addItem("onCheck");
    // добавить другие варианты здесь
    emit getMethod(0);
}

void Actions::loadJTextField(){
    ui->comboBox->clear();
    ui->comboBox->addItem("onEnter");
    ui->comboBox->addItem("onEditText");
    // добавить другие варианты здесь
    emit getMethod(0);
}

void Actions::loadJTextArea(){
    ui->comboBox->clear();
    ui->comboBox->addItem("onKeyPress");
    // добавить другие варианты здесь
    emit getMethod(0);
    
}

void Actions::loadJComboBox(){
    ui->comboBox->clear();
    ui->comboBox->addItem("onSelectItem");
    // добавить другие варианты здесь
    emit getMethod(0);

}


void Actions::loadMethod(int index, QString name, QStringList code){
    isLoad=true;
    ui->textEdit->clear();
    if(name=="#"){
        createCode(index);
        ui->checkBox->setChecked(false);
    } else {
        ui->label_4->setText(name);
        ui->textEdit->append(code.join("\n"));
        ui->checkBox->setChecked(true);
    }
    isLoad=false;
}

void Actions::createCode(int index){
    switch (Type) {
    case 4:
        ui->label_4->setText(Name+"_"+ui->comboBox->itemText(index)+"(int code)");
        ui->textEdit->append("private void "+ui->label_4->text()+" { // code of the pressed key");
        break;
    case 5:
        ui->label_4->setText(Name+"_"+ui->comboBox->itemText(index)+"(int index)");
        ui->textEdit->append("private void "+ui->label_4->text()+" { // index of the selected item");
        break;
    default:
        ui->label_4->setText(Name+"_"+ui->comboBox->itemText(index)+"()");
         ui->textEdit->append("private void "+ui->label_4->text()+" {");
        break;
    }

    ui->textEdit->append("        // TODO Auto-generated method stub");
    ui->textEdit->append("}");
}

void Actions::on_comboBox_currentIndexChanged(int index) //выбор метода из списка
{
    emit getMethod(index);
    setEnabledButtons(true);
}

void Actions::on_pushButton_clicked() // удалить метод из проекта
{
    ui->checkBox->setChecked(false);
    emit delMethod(ui->comboBox->currentIndex());
    setEnabledButtons(false);
}

void Actions::on_checkBox_clicked()
{
    ui->checkBox->setChecked(! ui->checkBox->isChecked());
}

void Actions::on_textEdit_textChanged() //редактирование кода
{
    if(isLoad)return;
    setEnabledButtons(true);
    QString line=ui->textEdit->toPlainText().split("\n")[0];
    int i=line.indexOf(ui->label_4->text());
    if(i<0){
        i=line.indexOf("void");
        if(i<0){
            QMessageBox::critical(NULL,QObject::tr("Error"),tr("Syntax error. There is no 'void' "));
            return;
        }
        line=line.mid(i+4,line.length()).trimmed();
        i=line.indexOf("{");
        if(i<0){
            QMessageBox::critical(NULL,QObject::tr("Error"),tr("Syntax error. There is no '{' "));
            return;
        }
        line=line.mid(0,i).trimmed();
        ui->label_4->setText(line);
    }
}
