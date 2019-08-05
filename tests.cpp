#include "tests.h"
#include "loadobj.h"
#include "triangle3d.h"
#include "ray.h"
#include "Octree/octree.h"

#include <QTest>

Tests::Tests(QObject *parent) : QObject(parent)
{

}

void Tests::readVertex01()
{
    QVector3D v;
    v[0] = 1.0f;
    v[1] = 2.0f;
    v[2] = 3.0f;
    QCOMPARE(LoadObj::readVertex("v 1 2 3", v), true);
    QCOMPARE(v, QVector3D(1, 2, 3));
}

void Tests::readVertex02()
{
    QVector3D v;
    v[0] = 1.0f;
    v[1] = 2.0f;
    v[2] = 3.0f;
    QCOMPARE(LoadObj::readVertex("v  1   2   3", v), true);
    QCOMPARE(v, QVector3D(1, 2, 3));
}

void Tests::readVertex03()
{
    QVector3D v;
    QCOMPARE(LoadObj::readVertex("v 1 2", v), false);
    QCOMPARE(v, QVector3D(0, 0, 0));
}

void Tests::readVertex04()
{
    QVector3D v;
    QCOMPARE(LoadObj::readVertex("v o p j", v), false);
    QCOMPARE(v, QVector3D(0, 0, 0));
}

void Tests::readVertex05()
{
    QVector3D v;
    QCOMPARE(LoadObj::readVertex("vt 1 2 3", v), false);
    QCOMPARE(v, QVector3D(0, 0, 0));
}

void Tests::RayIntersectionTriangle01()
{
    // Центрированный треугольник координат пересекается в 0-ой точке
    QVector3D p0 = QVector3D(-1.0f, 0.0f, 0.0f);
    QVector3D p1 = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D p2 = QVector3D(1.0f, 0.0f, 0.0f);

    QVector3D IntersectionPoints;
    Ray ray(QVector3D(0.0f, 0.0f, -0.6f), QVector3D(0.0f, 0.0f, 1.0f));
    QCOMPARE(Triangle3D::testIntersection(ray, IntersectionPoints, p0, p1, p2), true);
    QCOMPARE(IntersectionPoints, QVector3D(0.0f, 0.0f, 0.0f));
}

void Tests::RayIntersectionTriangle02()
{
    // Нецентрированный треугольник пересекается
    QVector3D p0 = QVector3D(2.0f, 0.0f, 0.0f);
    QVector3D p1 = QVector3D(3.5f, 2.2f, 0.0f);
    QVector3D p2 = QVector3D(3.9f, 0.0f, 0.0f);

    QVector3D IntersectionPoints;
    Ray ray(QVector3D(2.2f, 0.2f, -1.39f), QVector3D(0.0f, 0.0f, 1.0f));
    QCOMPARE(Triangle3D::testIntersection(ray, IntersectionPoints, p0, p1, p2), true);
}

void Tests::RayIntersectionTriangle03()
{
    // Луч перекрывает треугольник
    QVector3D p0 = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D p1 = QVector3D(1.0f, 1.0f, 0.0f);
    QVector3D p2 = QVector3D(1.0f, -1.0f, 0.0f);

    QVector3D IntersectionPoints;
    Ray ray(QVector3D(0.0f, -1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    QCOMPARE(Triangle3D::testIntersection(ray, IntersectionPoints, p0, p1, p2), false);
    QCOMPARE(IntersectionPoints, QVector3D(0.0f, 0.0f, 0.0f));
}

void Tests::RayIntersectionTriangle04()
{
    // Луч параллелен треугольнику
    QVector3D p0 = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D p1 = QVector3D(1.0f, 0.0f, -1.0f);
    QVector3D p2 = QVector3D(1.0f, 0.0f, 1.0f);

    QVector3D IntersectionPoints;
    Ray ray(QVector3D(0.0f, -1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f));
    QCOMPARE(Triangle3D::testIntersection(ray, IntersectionPoints, p0, p1, p2), false);
    QCOMPARE(IntersectionPoints, QVector3D(0.0f, 0.0f, 0.0f));

    ray.setRayDirection(QVector3D(0.0f, 0.0f, -1.0f));
    QCOMPARE(Triangle3D::testIntersection(ray, IntersectionPoints, p0, p1, p2), false);
    QCOMPARE(IntersectionPoints, QVector3D(0.0f, 0.0f, 0.0f));
}

void Tests::RayIntersectionTriangle05()
{
    // Нет пересечения
    QVector3D p0 = QVector3D(-1.0f, 0.0f, 0.0f);
    QVector3D p1 = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D p2 = QVector3D(1.0f, 0.0f, 0.0f);

    QVector3D IntersectionPoints;
    Ray ray(QVector3D(0.0f, 0.0f, -0.6f), QVector3D(0.0f, 0.0f, -1.0f));
    QCOMPARE(Triangle3D::testIntersection(ray, IntersectionPoints, p0, p1, p2), false);
    QCOMPARE(IntersectionPoints, QVector3D(0.0f, 0.0f, 0.0f));
}

void Tests::RayIntersectionAABB01()
{
    // Луч проходит напрямую через AABB
    Ray *ray = new Ray(QVector3D(1.53f, 2.33f, 0.21f), QVector3D(-1.21f, -1.87f, 0.0f));
    QCOMPARE(HelperFunctions::rayIntersectionAABB(*ray, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), true);
}

void Tests::RayIntersectionAABB02()
{
    // Луч касатеся грани AABB
    Ray *ray = new Ray(QVector3D(2.05f, 2.3f, 0.21f), QVector3D(-1.55f, -4.8f, 0.41f));
    QCOMPARE(HelperFunctions::rayIntersectionAABB(*ray, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), true);
}

void Tests::RayIntersectionAABB03()
{
    // Нет пересечения
    Ray *ray = new Ray(QVector3D(2.05f, 2.3f, 0.21f), QVector3D(0.39f, -4.23f, 0.41f));
    QCOMPARE(HelperFunctions::rayIntersectionAABB(*ray, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), false);
}

void Tests::RayIntersectionAABB04()
{
    // Луч проходит по углом в 45 градусов
    Ray *ray = new Ray(QVector3D(2.0f, -2.0f, 2.0f), QVector3D(-4.0f, 4.0f, -4.0f));
    QCOMPARE(HelperFunctions::rayIntersectionAABB(*ray, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), true);
}

void Tests::TriangleInAABB01()
{
    // Треугольник под 45 градусов
    QCOMPARE(HelperFunctions::triangleInsideAABB(QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, -1.0f), QVector3D(-1.0f, -1.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), true);
}

void Tests::TriangleInAABB02()
{
    // Маленький треугольник
    QCOMPARE(HelperFunctions::triangleInsideAABB(QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, -1.0f), QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), true);
}

void Tests::TriangleInAABB03()
{
    // Треугольник выходит за пределы AABB
    QCOMPARE(HelperFunctions::triangleInsideAABB(QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, -1.0f), QVector3D(-2.0f, -2.0f, 2.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), true);
}

void Tests::TriangleInAABB04()
{
    // Треугольник вне (и не содержит в себе) AABB
    QCOMPARE(HelperFunctions::triangleInsideAABB(QVector3D(-2.0f, -2.0f, -2.0f), QVector3D(2.0f, -3.0f, 0.0f), QVector3D(2.0f, -2.0f, -2.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), false);
}

void Tests::TriangleInAABB05()
{
    // Треугольник содержит в себе AABB
    QCOMPARE(HelperFunctions::triangleInsideAABB(QVector3D(-2.0f, -2.0f, -2.0f), QVector3D(2.0f, 1.0f, 2.0f), QVector3D(2.0f, 1.0f, -2.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f)), true);
}

void Tests::RayIntersectionOctree01()
{
    // Луч попадает в AABB и проходит все (2 треугольника)

    QVector<QVector3D> points;
    QVector<int> faces;
    Ray ray;

    QVector3D p0 = QVector3D(-1.0f, 0.0f, 1.0f);
    QVector3D p1 = QVector3D(-1.0f, 1.0f, 0.0f);
    QVector3D p2 = QVector3D(-1.0f, -1.0f, 0.0f);

    QVector3D p3 = QVector3D(1.0f, 0.0f, 1.0f);
    QVector3D p4 = QVector3D(1.0f, 1.0f, 0.0f);
    QVector3D p5 = QVector3D(1.0f, -1.0f, 0.0f);

    QVector3D start = QVector3D(-1.83f, 0.23f, 0.36f);
    QVector3D dist = QVector3D(4.99f, -0.11f, 0.14f);

    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);

    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);

    faces.push_back(0 + 1);
    faces.push_back(1 + 1);
    faces.push_back(2 + 1);
    faces.push_back(3 + 1);
    faces.push_back(4 + 1);
    faces.push_back(5 + 1);

    ray.setRayOrigin(start);
    ray.setRayDirection(dist);

    Octree octre(points, faces);

    Octree tmpOctree(points, faces);
    QCOMPARE(tmpOctree.intersection(ray), true);
}

void Tests::RayIntersectionOctree02()
{
    // Луч попадает в AABB, и проходит дальний треугольник

    QVector<QVector3D> points;
    QVector<int> faces;
    Ray ray;

    QVector3D p0 = QVector3D(-1.0f, 0.0f, 1.0f);
    QVector3D p1 = QVector3D(-1.0f, 1.0f, 0.0f);
    QVector3D p2 = QVector3D(-1.0f, -1.0f, 0.0f);

    QVector3D p3 = QVector3D(1.0f, 0.0f, 1.0f);
    QVector3D p4 = QVector3D(1.0f, 1.0f, 0.0f);
    QVector3D p5 = QVector3D(1.0f, -1.0f, 0.0f);

    QVector3D start = QVector3D(-2.34f, -1.1f, 0.5f);
    QVector3D dist = QVector3D(5.0f, 1.21f, 0.0f);

    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);

    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);

    faces.push_back(0 + 1);
    faces.push_back(1 + 1);
    faces.push_back(2 + 1);
    faces.push_back(3 + 1);
    faces.push_back(4 + 1);
    faces.push_back(5 + 1);

    ray.setRayOrigin(start);
    ray.setRayDirection(dist);

    Octree octre(points, faces);

    Octree tmpOctree(points, faces);
    QCOMPARE(tmpOctree.intersection(ray), true);
}

void Tests::RayIntersectionOctree03()
{
    // Луч проходит между треугольниками
    QVector<QVector3D> points;
    QVector<int> faces;
    Ray ray;

    QVector3D p0 = QVector3D(-1.0f, 0.0f, 1.0f);
    QVector3D p1 = QVector3D(-1.0f, 1.0f, 0.0f);
    QVector3D p2 = QVector3D(-1.0f, -1.0f, 0.0f);

    QVector3D p3 = QVector3D(1.0f, 0.0f, 1.0f);
    QVector3D p4 = QVector3D(1.0f, 1.0f, 0.0f);
    QVector3D p5 = QVector3D(1.0f, -1.0f, 0.0f);

    QVector3D start = QVector3D(-1.02f, -2.83f, 0.5f);
    QVector3D dist = QVector3D(1.91f, 4.93f, -0.29f);

    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);

    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);

    faces.push_back(0 + 1);
    faces.push_back(1 + 1);
    faces.push_back(2 + 1);
    faces.push_back(3 + 1);
    faces.push_back(4 + 1);
    faces.push_back(5 + 1);

    ray.setRayOrigin(start);
    ray.setRayDirection(dist);

    Octree octre(points, faces);

    Octree tmpOctree(points, faces);
    QCOMPARE(tmpOctree.intersection(ray), false);
}
