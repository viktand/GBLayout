#include "entertext.h"
#include "ui_entertext.h"

enterText::enterText(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::enterText)
{
    ui->setupUi(this);
}

enterText::~enterText()
{
    delete ui;
}

void enterText::setLabel(QString s){
    ui->label->setText(s);
    ui->lineEdit->setFocus();
}

void enterText::clearText(){
    ui->lineEdit->clear();
}

void enterText::setText(QString s){
    ui->lineEdit->setText(s);
}

void enterText::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok){
        emit okPress(ui->lineEdit->text(), types);
    }
}
