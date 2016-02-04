#ifndef QAVCHECKBOX_H
#define QAVCHECKBOX_H

#include <QCheckBox>
#include <QMouseEvent>

class QavCheckbox : public QCheckBox
{
    Q_OBJECT
public:
    explicit QavCheckbox(QWidget *parent = 0);

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

#endif // QAVCHECKBOX_H
