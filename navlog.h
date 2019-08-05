#ifndef NAVLOG_H
#define NAVLOG_H

#include <QObject>

#include "widget.h"
#include "octree.h"
#include "ray.h"
#include "arcball.h"

class NavLog : public QObject
{

    Q_OBJECT
public:
    NavLog(Widget *widget, Camera3D *camera, QVector<Octree *> octree);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
protected:
    QVector3D getRayFromMouseCoordinates(const QVector2D &mousePos);
private:
    Widget *m_widget;
    Camera3D *m_camera;
    QVector2D m_mousePos;
    QVector<Octree *> m_octree;

    Arcball *arcball;
};

#endif // NAVLOG_H
