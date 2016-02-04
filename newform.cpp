#include "newform.h"
#include "ui_newform.h"
//#include "QAbstractButton"

newForm::newForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newForm)
{
    ui->setupUi(this);
}

newForm::~newForm()
{
    delete ui;
}

void newForm::on_buttonBox_clicked(QAbstractButton *button)                         // обработка нажатия кнопок
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok){             // кнопка Ок
        emit createNewForm(ui->spinBox->value(), ui->spinBox_2->value(), ui->lineEdit->text());
    }
}
