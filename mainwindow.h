#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <avLabel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    virtual void  resizeEvent(QResizeEvent *);

private slots:
    void jComponents();                                 // нарисовать заготовки компонентов
    void setResizer();                                  // создать ручку-изменялку размера окна (resizer)
    void resizerPress(int x, int y, int i);
    void resizerMove(int x, int y, int i);
    void resizerUp(int x, int y, int i);     
    void titleMove(int x, int y, int i);                // перемещение заготовки окна
    void createform(int X, int Y, QString s);           // создать новую форму
    void compPress(int x, int y, int i);                // работа comp с мышкой (перемещение)
    void compMove(int x, int y, int i);
    void compUp(int x, int y, int i);
    void cmsPress(int x, int y, int i);                 // работа cms с мышкой
    void cmsMove(int x, int y, int i);
    void cmsUp(int x, int y, int i);
    void cmsRClick(int x, int y, int i);
    QPoint getGridPosition(QavLabel *avl, bool b);      // вернуть координаты ячейки, в которую попадает avLabel
    // menu
    void setMenu();
    void setSizeComp();                                 // Задать размер
    void setTypeJButton();
    void setTypeJCheckbox();
    void setTypeJTextField();
    void setTypeJTextArea();
    void setTypeJLabel();
    void setTypeJComboBox();
    void setTypeComp(int index);                        // Задать тип
    void setTextComp();                                 // установить текст компонента
    void setNameComp();                                 // установить свое имя компонента
    void setTextComp_end(QString s, int t);
    void incrHor();                                     // увеличить компонент на одну ячейку по горизонтале
    void decrHor();                                     // уменьшить компонент на одну ячейку по горизонтале
    void incrVer();                                     // увеличить компонент на одну ячейку по вертик.
    void decrVer();                                     // уменьшить компонент на одну ячейку по вертик.
    void delComponent();                                // удалить компонент
    void on_pushButton_clicked();
    void setFillNone();
    void setFillBoth();
    void setFillHor();
    void setFillVer();
    void setAnchorCen();
    void setAnchorNorth();
    void setAnchorSouth();
    void setAnchorEast();
    void setAnchorWest();
    void setAnchorNorthEast();
    void setAnchorSouthWest();
    void setAnchorSouthEast();
    void setAnchorNorthWest();
    void setInsets();
    void setInsets_end(QRect r);
    void addMethod();
    void addMethod_end(int act, QString nm, QStringList sl); // сохранить метод
    void delMethod(int index);                               // удалить метод
    void loadMethod(int index);                              // загрузить метод в форму редактирования, если он существует
    void clearIfNeed(int act);                               // удалить метод index, если он уже был
    void on_pushButton_2_clicked();
    QString getTextFill(int index);                           // получить текстовое значение fill
    QString getTextAnchor(int index);
    QString getTextType(int index);
    void resizeGrid();                                        // перестроить сетку при изменении размера окна
    void drawGrid();                                          // нарисовать сетку
    QString getPictureName(int index);                        // получить имя ресурса картинки компонента по его индексу
    void on_checkBox_2_clicked(bool checked);
    QString getClearName(QString fn);                         // получить "чистое" имя функции, т.е. без "()" в конце
    QStringList getListener(int index, int name);             // получить код слушателя для компонета и события
    QStringList getButtonsListener(int index, int name);      // получить код слушателя для кнопки
    QStringList getTextFieldListener(int index, int name);    // получить код слушателя для текстового поля ввода
    QStringList getTextAreaListener(int index, int name);     // получить код слушателя для JTextArea
    QStringList getComboBoxListener(int index, int name);     // получить код слушателя для JComboBox
    QStringList getImportComp(int i);                         // получить строку импорта для компонента
    QStringList getImports(int index);                        // получить список импортов для слушателя index
    QStringList addLines(QStringList sl, QStringList l);      // Добавить к sl строки из l, которых нет в sl и вернуть sl
    void copyComponent();                                     // копировать компонент в буфер программы
    void pasteComponent();                                    // вставить компонент из буфера программы
    void setMenuWindow();                                     // создать меню окна
    void openMenuWindow(int x, int y, int index);             // открыть меню окна
};

#endif // MAINWINDOW_H
