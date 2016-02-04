#ifndef ACTIONS_H
#define ACTIONS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Actions;
}

class Actions : public QDialog
{
    Q_OBJECT

public:
    explicit Actions(QWidget *parent = 0);
    ~Actions();
    void setType(int type, QString name);       // задать тип и имя компонента
    void loadMethod(int index, QString name, QStringList code);  // загрузить метод из компонента

private:
    Ui::Actions *ui;
    void loadActions();                              // загрузить варианты action для этого компонента
    void loadJButton();
    void loadJCheckBox();
    void loadJTextField();
    void loadJTextArea();
    void loadJComboBox();

    void createCode(int index);                     // генерировать код
    void setEnabledButtons(bool b);                 // разрешить/запретить кнопки buttonbox

signals:
    void setMethod(int act, QString mn, QStringList sl);    // сигнал сохранить новый метод
    void getMethod(int index);                              // запросить текст метода index (если редактировать)
    void delMethod(int index);                              // удалить метод

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_checkBox_clicked();
    void on_textEdit_textChanged();
};

#endif // ACTIONS_H
