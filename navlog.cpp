#include "navlog.h"

#include <QMouseEvent>
#include <QWheelEvent>

NavLog::NavLog(Widget *widget, Camera3D *camera, QVector<Octree *> octree)
{
    m_widget = widget;
    m_camera = camera;
    m_octree = octree;

    arcball = new Arcball(m_widget->width(), m_widget->height());
}

bool NavLog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_widget && event->type() == QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
        if(wheelEvent->delta() > 0)
        {
            m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
            m_widget->update();
            return true;
        }
        else if (wheelEvent->delta() < 0)
        {
            m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
            m_widget->update();
            return true;
        }
        else
            return false;
    }

    if (obj == m_widget && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->buttons() == Qt::LeftButton)
        {
            m_mousePos = QVector2D(mouseEvent->localPos());

            arcball->startDrag(m_mousePos);

            m_widget->update();
            return true;
        }
        else if(mouseEvent->buttons() == Qt::MiddleButton)
        {
            m_mousePos = QVector2D(mouseEvent->localPos());
            m_widget->update();
            return true;
        }
        else if (mouseEvent->button() == Qt::RightButton)
        {
            m_mousePos = QVector2D(mouseEvent->localPos());

            QVector3D start = QVector3D(0.0f, 0.0f, 5.0f);
            QVector3D dist = getRayFromMouseCoordinates(m_mousePos) * m_camera->getZFar();

            Ray ray(start, dist);

            for (int i = 0; i < m_octree.size(); i++)
                if (m_octree[i]->intersection(ray))
                    qDebug() << "true" << endl;
                else
                    qDebug() << "false" << endl;

            m_widget->update();
            return true;
        }
        else
            return false;
    }

    if (obj == m_widget && event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->buttons() == Qt::LeftButton)
        {
            QVector2D diff = QVector2D(mouseEvent->localPos());

            arcball->drag(diff);

            m_camera->rotate(arcball->getQuternion());

            arcball->change();
//            m_mousePos = QVector2D(mouseEvent->localPos());

//            float angleX = diff.y() / 2.0f, angleY = diff.x() / 2.0f;

//            m_camera->rotateX(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY));
//            m_camera->rotateY(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX));

            m_widget->update();
            return true;
        }
        else
            return false;
    }
    return false;
}

QVector3D NavLog::getRayFromMouseCoordinates(const QVector2D &mousePos)
{
    float x = (2.0f * mousePos.x()) / m_widget->width() - 1.0f;
    float y = 1.0f - (2.0f * mousePos.y()) / m_widget->height();
    float z = 1.0f;

    QVector3D ray_nds(x, y, z);

    QVector4D ray_clip(ray_nds.x(), ray_nds.y(), -1.0f, 1.0f);
    QVector4D ray_eye = m_camera->getProjectionMatrix().inverted() * ray_clip;

    ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1.0f, 0.0f);

    QVector3D ray_world = (m_camera->getViewMatrix().inverted() * ray_eye).toVector3D().normalized();

    return ray_world;
}
