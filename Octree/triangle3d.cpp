#include "triangle3d.h"

#include "helperfunctions.h"
#include "ray.h"

Triangle3D::Triangle3D()
{
    p0 = QVector3D(0.0f, 0.0f, 0.0f);
    p1 = QVector3D(0.0f, 0.0f, 0.0f);
    p2 = QVector3D(0.0f, 0.0f, 0.0f);
}

Triangle3D::Triangle3D(const QVector3D &_p0, const QVector3D &_p1, const QVector3D &_p2)
{
    p0 = _p0;
    p1 = _p1;
    p2 = _p2;
}

Triangle3D::~Triangle3D()
{

}

bool Triangle3D::intersection(const Ray &ray, QVector3D &IntersectionPoint, bool Cull_Face)
{
    QVector3D edge1 = p1 - p0;
    QVector3D edge2 = p2 - p0;

    QVector3D h = QVector3D::crossProduct(ray.getRayDirection(), edge2);

    float det = QVector3D::dotProduct(edge1, h);

    if (Cull_Face)
    {
        if (det < HelperFunctions::EPSILON)
            return false;
    }
    else
    {
        if (det > -HelperFunctions::EPSILON && det < HelperFunctions::EPSILON)
            return false;    // This ray is parallel to this triangle.
    }

    float inv_det = 1.0f / det;
    QVector3D s = ray.getRayOrigin() - p0;
    float u = inv_det * QVector3D::dotProduct(s, h);
    if (u < 0.0f || u > 1.0f)
        return false;

    QVector3D q = QVector3D::crossProduct(s, edge1);
    float v = inv_det * QVector3D::dotProduct(ray.getRayDirection(), q);
    if (v < 0.0f || (u + v) > 1.0f)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * QVector3D::dotProduct(edge2, q);

    if (t > HelperFunctions::EPSILON) // ray intersection
    {
        IntersectionPoint = ray.getPoint(t);
        return true;
    }

    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

QVector3D Triangle3D::getP0()
{
    return p0;
}

QVector3D Triangle3D::getP1()
{
    return p1;
}

QVector3D Triangle3D::getP2()
{
    return p2;
}

bool Triangle3D::testIntersection(const Ray &ray, QVector3D &IntersectionPoint, const QVector3D &p0, const QVector3D &p1, const QVector3D &p2)
{
    QVector3D edge1 = p1 - p0;
    QVector3D edge2 = p2 - p0;

    QVector3D h = QVector3D::crossProduct(ray.getRayDirection(), edge2);

    float det = QVector3D::dotProduct(edge1, h);
    if (det > -HelperFunctions::EPSILON && det < HelperFunctions::EPSILON)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0f / det;
    QVector3D s = ray.getRayOrigin() - p0;
    float u = inv_det * QVector3D::dotProduct(s, h);
    if (u < 0.0f || u > 1.0f)
        return false;

    QVector3D q = QVector3D::crossProduct(s, edge1);
    float v = inv_det * QVector3D::dotProduct(ray.getRayDirection(), q);
    if (v < 0.0f || (u + v) > 1.0f)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * QVector3D::dotProduct(edge2, q);

    if (t > HelperFunctions::EPSILON) // ray intersection
    {
        IntersectionPoint = ray.getPoint(t);
        return true;
    }

    else // This means that there is a line intersection but not a ray intersection.
        return false;
}
