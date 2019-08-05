#ifndef RAY_H
#define RAY_H

#include <QVector3D>

class Ray
{
public:
    Ray() :
        origin(QVector3D(0.0f, 0.0f, 0.0f)), direction(QVector3D(0.0f, 0.0f, 0.0f))
    {
    }

    Ray(QVector3D orig, QVector3D dir) :
        origin(orig), direction(dir)
    {
    }

    void setRayOrigin(const QVector3D &_origin) { origin = _origin; }
    void setRayDirection(const QVector3D &_direction) { direction = _direction; }

    QVector3D getRayOrigin() const { return origin; }
    QVector3D getRayDirection() const { return direction; }

    QVector3D getPoint(float t) const { return origin + t * direction; }
private:
    QVector3D origin, direction;
};

#endif // RAY_H
