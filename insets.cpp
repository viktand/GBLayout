#include "insets.h"
#include "ui_insets.h"
#include <QAbstractButton>

bool notPress=true;

Insets::Insets(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Insets)
{
    ui->setupUi(this);
}

Insets::~Insets()
{
    delete ui;
}

void Insets::setValue(QRect r){
    ui->spinBox_2->setValue(r.x());
    ui->spinBox->setValue(r.y());
    ui->spinBox_3->setValue(r.width());
    ui->spinBox_4->setValue(r.height());
}

void Insets::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok){
        emit setInsets(QRect(ui->spinBox_2->value(),
                             ui->spinBox->value(),
                             ui->spinBox_3->value(),
                             ui->spinBox_4->value()));
    }
}

void Insets::on_pushButton_clicked()
{
    notPress=false;
    ui->spinBox->setValue(0);
    ui->spinBox_2->setValue(0);
    ui->spinBox_3->setValue(0);
    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);
    notPress=true;
}

void Insets::on_spinBox_5_valueChanged(int arg1)
{
    if(notPress){
        ui->spinBox->setValue(ui->spinBox_5->value());
        ui->spinBox_2->setValue(ui->spinBox_5->value());
        ui->spinBox_3->setValue(ui->spinBox_5->value());
        ui->spinBox_4->setValue(ui->spinBox_5->value());
    }
}
