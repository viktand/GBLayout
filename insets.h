#ifndef INSETS_H
#define INSETS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Insets;
}

class Insets : public QDialog
{
    Q_OBJECT

public:
    explicit Insets(QWidget *parent = 0);
    ~Insets();
    void setValue(QRect r);

private:
    Ui::Insets *ui;

signals:
    void setInsets(QRect ins);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_pushButton_clicked();
    void on_spinBox_5_valueChanged(int arg1);
};

#endif // INSETS_H
