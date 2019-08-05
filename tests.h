#ifndef TESTS_H
#define TESTS_H

#include <QObject>

class Tests : public QObject
{
    Q_OBJECT
public:
    explicit Tests(QObject *parent = nullptr);
private slots:
    // Для проверки загрузки вершин
    void readVertex01();
    void readVertex02();
    void readVertex03();
    void readVertex04();
    void readVertex05();
    //

    // Для проверки пересечения Луча и треугольника
    void RayIntersectionTriangle01();
    void RayIntersectionTriangle02();
    void RayIntersectionTriangle03();
    void RayIntersectionTriangle04();
    void RayIntersectionTriangle05();
    //

    // Для проверки Ray Intersection AABB
    void RayIntersectionAABB01();
    void RayIntersectionAABB02();
    void RayIntersectionAABB03();
    void RayIntersectionAABB04();
    //

    // Для проверки Triangle Inside AABB
    void TriangleInAABB01();
    void TriangleInAABB02();
    void TriangleInAABB03();
    void TriangleInAABB04();
    void TriangleInAABB05();
    //

    // Для проверки Octree
    void RayIntersectionOctree01();
    void RayIntersectionOctree02();
    void RayIntersectionOctree03();
    //
};

#endif // TESTS_H
