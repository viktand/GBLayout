#ifndef AVLABEL_H
#define AVLABEL_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include "qavbutton.h"
#include "qavcheckbox.h"


class QavLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int Imnum READ getImnum WRITE setImnum) // свойство - номер объекта (чтобы не вычислять)
    Q_PROPERTY(int curComp READ getComponent WRITE setComponent) // свойство - индекс (тип) дочернего компонента
    Q_PROPERTY(QString textComp READ getTextComponent WRITE setTextComponent) // свойство - текст на компоненте
    Q_PROPERTY(int Jfill READ getFill WRITE setFill) // свойство - заполнение
    Q_PROPERTY(int Janchor READ getAnchor WRITE setAnchor) // свойство - заполнение
    Q_PROPERTY(QRect Jinsets READ getInsets WRITE setInsets) // свойство - заполнение


    private:
        int     Imnum;       // номер объекта к которому привязан лабел
        int     curComp;     // индекс (тип) дочернего компонента
        QString textComp;    // текст на компоненте
        void clearComp();    // удалить компоненты
        int     Jfill;
        int     Janchor;
        QRect   Jinsets;
        void selectFill(int i);
        void setDxDy();
        //QPushButton *pb;
        QavButton   *pb;
        QavCheckbox *chb;
        QLabel      *lb;
        QLineEdit   *le;
        QTextEdit   *te;
        QComboBox   *cb;

    public:
        QavLabel(QWidget *parent = 0);
        ~QavLabel();
        int  getImnum()   {  return Imnum;  }
        void setImnum(int value) { Imnum=value;}

        void setComponent(int index);           // установить компонент поверх лабела
        int getComponent(){
            return curComp;
        }

        void setTextComponent(QString s);       // установить текст компонента
        QString getTextComponent(){
            return textComp;
        }

        void setFill(int index);       // установить заполнение
        int getFill(){
            return Jfill;
        }

        void setAnchor(int index);       // установить выравнивание
        int getAnchor(){
            return Janchor;
        }

        void setInsets(QRect ins);       // установить отступы
        QRect getInsets(){
            return Jinsets;
        }

        void setEnabledComponent(bool en); // установить режим компонента


    signals:
        void mouse_up(int x, int y, int i);
        void rclicked(int x, int y, int i);
        void mouse_press(int x, int y, int i);
        void mouse_move(int x, int y, int i);
        void mouse_wheel(int i, int index);

    protected:
        void mouseReleaseEvent(QMouseEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        void wheelEvent(QWheelEvent *event);

private slots:
        void mouseRelease(QMouseEvent *e);
        void mousePress(QMouseEvent *e);
        void mouseMove(QMouseEvent *e);
        void wheelEv(QWheelEvent *e);
};
#endif // AVLABEL_H
