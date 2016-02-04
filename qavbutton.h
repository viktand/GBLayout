#ifndef QAVBUTTON_H
#define QAVBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class QavButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QavButton(QWidget *parent = 0);

signals:
    void mouse_Release(QMouseEvent *e);
    void mouse_Press(QMouseEvent *e);
    void mouse_Move(QMouseEvent *e);
    void wheel_Event(QWheelEvent *e);


public slots:

protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
};

#endif // QAVBUTTON_H
