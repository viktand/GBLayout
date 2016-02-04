#ifndef ENTERTEXT_H
#define ENTERTEXT_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class enterText;
}

class enterText : public QDialog
{
    Q_OBJECT

public:
    explicit enterText(QWidget *parent = 0);
    ~enterText();
    void setLabel(QString s);
    void clearText();
    void setText(QString s);
    int types;

private:
    Ui::enterText *ui;

signals:
    void okPress(QString s, int t);        // сигнал нажатия на Ok
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // ENTERTEXT_H
