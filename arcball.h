#ifndef ARCBALL_H
#define ARCBALL_H

#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include <QtMath>

class Arcball
{
public:
    Arcball(int _screenWidth, int _screenHeight);

    void startDrag(const QVector2D &mousePos);
    void drag(const QVector2D &mousePos);
    void updateSize(int _screenWidth, int _screenHeight);

    void change();

    QQuaternion getQuternion();
private:
    QVector3D mouseCoordsToSphereCoords(const QVector3D &point);

    int screenWidth;
    int screenHeight;

    float radius;

    QVector3D from, to;

    QQuaternion resQuat;
};

#endif // ARCBALL_H
