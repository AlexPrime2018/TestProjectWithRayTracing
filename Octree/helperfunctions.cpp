#include "helperfunctions.h"

bool HelperFunctions::pointInsideAABB(const QVector3D &point, const QVector3D &center, const QVector3D &size) {
    QVector3D min = center - size;
    QVector3D max = center + size;

    return (point.x() >= min.x() && point.x() <= max.x() && point.y() >= min.y() && point.y() <= max.y() && point.z() >= min.z() && point.z() <= max.z());
}

bool HelperFunctions::rayIntersectionAABB(const Ray &ray, const QVector3D &center, const QVector3D &size) {
    QVector3D min = center - size;
    QVector3D max = center + size;

    float tmin = (min.x() - ray.getRayOrigin().x()) / ray.getRayDirection().x();
    float tmax = (max.x() - ray.getRayOrigin().x()) / ray.getRayDirection().x();

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min.y() - ray.getRayOrigin().y()) / ray.getRayDirection().y();
    float tymax = (max.y() - ray.getRayOrigin().y()) / ray.getRayDirection().y();

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (min.z() - ray.getRayOrigin().z()) / ray.getRayDirection().z();
    float tzmax = (max.z() - ray.getRayOrigin().z()) / ray.getRayDirection().z();

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}

//bool HelperFunctions::rayIntersectionAABB(const Ray &ray, const QVector3D &center, const QVector3D &size, float Maxdist) {
//   return rayIntersectionAABB(ray, center, size, &Maxdist);
//}

bool HelperFunctions::triangleInsideAABB(const QVector3D &p0, const QVector3D &p1, const QVector3D &p2, const QVector3D &center, const QVector3D &size) {
    if (pointInsideAABB(p0, center, size))
        return true;

    if (pointInsideAABB(p1, center, size))
        return true;

    if (pointInsideAABB(p2, center, size))
        return true;

    QVector3D vert[3];
    QVector3D norm[3];
    QVector3D face[3];

    // Помещаем в начало (локальных) координат
    vert[0] = p0 - center;
    vert[1] = p1 - center;
    vert[2] = p2 - center;

    // Получаем нормали
    norm[0].setX(1.0f);
    norm[0].setX(0.0f);
    norm[0].setX(0.0f);

    norm[1].setX(0.0f);
    norm[1].setX(1.0f);
    norm[1].setX(0.0f);

    norm[2].setX(0.0f);
    norm[2].setX(0.0f);
    norm[2].setX(1.0f);

    //Получаем треугольники по трём вершинам
    face[0] = vert[1] - vert[0];
    face[1] = vert[2] - vert[1];
    face[2] = vert[0] - vert[2];

    Ray testRay(p0, face[0]);

    if (rayIntersectionAABB(testRay, center, size))
        return true;

    testRay.setRayOrigin(p1);
    testRay.setRayDirection(face[1]);

    if (rayIntersectionAABB(testRay, center, size))
        return true;

    testRay.setRayOrigin(p2);
    testRay.setRayDirection(face[2]);

    if (rayIntersectionAABB(testRay, center, size))
        return true;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            QVector3D a = QVector3D::crossProduct(norm[i], face[j]);

            float t0 = QVector3D::dotProduct(a, vert[0]);
            float t1 = QVector3D::dotProduct(a, vert[1]);
            float t2 = QVector3D::dotProduct(a, vert[2]);

            float min = std::min(std::min(t0, t1), t2);
            float max = std::max(std::max(t0, t1), t2);

            float x = a.x() < 0.0f ? -a.x() : a.x();
            float y = a.y() < 0.0f ? -a.y() : a.y();
            float z = a.z() < 0.0f ? -a.z() : a.z();

            a.setX(x);
            a.setY(y);
            a.setZ(z);

            float rad = QVector3D::dotProduct(size, a);
            if (min > rad || max < -rad)
                return false;
        }
    }

    return true;
}
