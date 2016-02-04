#include "qavbutton.h"

QavButton::QavButton(QWidget *parent) : QPushButton(parent)
{

}

void QavButton::mouseReleaseEvent(QMouseEvent *e){
    emit mouse_Release(e);
    QPushButton::mouseReleaseEvent(e);
}

void QavButton::mousePressEvent(QMouseEvent *e){
    emit mouse_Press(e);
    QPushButton::mousePressEvent(e);
}

void QavButton::mouseMoveEvent(QMouseEvent *e){
    emit mouse_Move(e);
    QPushButton::mouseMoveEvent(e);
}

void QavButton::wheelEvent(QWheelEvent *e){
    emit wheel_Event(e);
    QPushButton::wheelEvent(e);
}
