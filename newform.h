#ifndef NEWFORM_H
#define NEWFORM_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class newForm;
}

class newForm : public QDialog
{
    Q_OBJECT

public:
    explicit newForm(QWidget *parent = 0);
    ~newForm();

private:
    Ui::newForm *ui;

signals:
    void createNewForm(int X, int Y, QString name);     // сигнал создания новой формы

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

};

#endif // NEWFORM_H
