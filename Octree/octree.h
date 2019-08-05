#ifndef OCTREE_H
#define OCTREE_H

#include <QVector3D>
#include <QVector>

#include "helperfunctions.h"
#include "tests.h"
#include "triangle3d.h"

class OctreeNode;
class Ray;

class Octree
{
public:
    Octree(const QVector<QVector3D> &points, const QVector<int> &faces);

    bool intersection(const Ray& ray);

    void clear();

    Triangle3D getTriangle();
private:
    OctreeNode* m_root;

    QVector<QVector3D> m_points;
    QVector<int> m_faces;

    void makeTriangle();
    void addTriangle(Triangle3D *tri);

    Triangle3D closestTriangl;

    void setRootCenterAndSize();
};

#endif // OCTREE_H
