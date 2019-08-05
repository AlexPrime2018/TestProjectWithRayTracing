#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include <QVector3D>

#include "tests.h"

class Ray;

class Triangle3D
{
public:
    Triangle3D();
    Triangle3D(const QVector3D &_p0, const QVector3D &_p1, const QVector3D &_p2);
    ~Triangle3D();

    bool intersection(const Ray &ray, QVector3D &IntersectionPoint, bool Cull_Face = true);

    QVector3D getP0();
    QVector3D getP1();
    QVector3D getP2();
private:
    friend class Tests;
    static bool testIntersection(const Ray &ray, QVector3D &IntersectionPoint, const QVector3D &p0, const QVector3D &p1, const QVector3D &p2);

    QVector3D p0, p1, p2;
};

#endif // TRIANGLE3D_H
