#include "qavcheckbox.h"

QavCheckbox::QavCheckbox(QWidget *parent): QCheckBox(parent)
{

}

void QavCheckbox::mouseReleaseEvent(QMouseEvent *e){
    emit mouse_Release(e);
    QCheckBox::mouseReleaseEvent(e);
}

void QavCheckbox::mousePressEvent(QMouseEvent *e){
    emit mouse_Press(e);
    QCheckBox::mousePressEvent(e);
}

void QavCheckbox::mouseMoveEvent(QMouseEvent *e){
    emit mouse_Move(e);
    QCheckBox::mouseMoveEvent(e);
}

void QavCheckbox::wheelEvent(QWheelEvent *e){
    emit wheel_Event(e);
    QCheckBox::wheelEvent(e);
}

