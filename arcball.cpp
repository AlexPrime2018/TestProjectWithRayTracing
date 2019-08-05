#include "arcball.h"

Arcball::Arcball(int _screenWidth, int _screenHeight)
    : screenWidth(_screenWidth), screenHeight(_screenHeight)
{
    radius = ((std::min(screenWidth, screenHeight) / 2.0f) - 1);
}

void Arcball::startDrag(const QVector2D &mousePos)
{
    QVector3D point(mousePos.x(), mousePos.y(), 0.0f);
    from = mouseCoordsToSphereCoords(point);
}

void Arcball::drag(const QVector2D &mousePos)
{
    QVector3D point(mousePos.x(), mousePos.y(), 0.0f); // Pegando o ponto que está na tela
    point.setZ(0.0f);
    to = mouseCoordsToSphereCoords(point);
    QVector3D pt0(from.x(),from.y(),from.z());
    QVector3D pt1(to.x(),to.y(),to.z());
    QQuaternion Q0(0.0f, pt0);
    QQuaternion Q1(0.0f, pt1);
    QQuaternion QP = Q0.conjugated();
    resQuat = Q1 * QP;
}

void Arcball::updateSize(int _screenWidth, int _screenHeight)
{
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;

    radius = ((std::min(screenWidth, screenHeight) / 2.0f) - 1);
}

void Arcball::change()
{
    from = to;
}

QQuaternion Arcball::getQuternion()
{
    return resQuat;
}

QVector3D Arcball::mouseCoordsToSphereCoords(const QVector3D &point)
{
    QVector3D pointf;
    float r,s;

    pointf.setX((point.x()-(screenWidth / 2)) / radius);
    pointf.setY((point.y()-(screenHeight / 2)) / radius);
    r= pointf.x()*pointf.x()+pointf.y()*pointf.y();

    if(r > 1.0f)
    {
        s= 1.0f / qSqrt(r);
        pointf.setX(s*pointf.x());
        pointf.setY(s*pointf.y());
        pointf.setZ(0.0f);
    }
    else
    {
        pointf.setZ(qSqrt(1.0f - r));
    }
    return pointf;
}
