#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <QVector3D>
#include <QtMath>

#include "ray.h"

namespace  HelperFunctions {

bool pointInsideAABB(const QVector3D &point, const QVector3D &center, const QVector3D &size);

bool rayIntersectionAABB(const Ray &ray, const QVector3D &center, const QVector3D &size);

//bool rayIntersectionAABB(const Ray &ray, const QVector3D &center, const QVector3D &size, float Maxdist);

bool triangleInsideAABB(const QVector3D &p0, const QVector3D &p1, const QVector3D &p2, const QVector3D &center, const QVector3D &size);

const float EPSILON = std::numeric_limits<float>::epsilon();
}

#endif // HELPERFUNCTIONS_H
