#ifndef OCTREENODE_H
#define OCTREENODE_H

#include <QVector3D>
#include <QVector>

#include "helperfunctions.h"

class Triangle3D;
class Ray;

class OctreeNode
{
public:
    OctreeNode(const QVector3D& center, const QVector3D& size, const unsigned int& level, OctreeNode* parent = nullptr);
    ~OctreeNode();

    bool intersection(const Ray& ray, Triangle3D *closestTriangle = nullptr);
    void addTriangle(Triangle3D* triangles);
private:
    QVector3D m_center;
    QVector3D m_size;
    QVector3D IntersectionPoint;

    unsigned int m_level;

    OctreeNode* m_parent;
    OctreeNode* m_child[8];
    QVector3D m_childCenter[8];

    QVector<Triangle3D*> m_trianglesThisNode;

    static unsigned int LEVEL_MAX;
    static unsigned int LIMIT_MAX;
    bool m_limitReached;
};

#endif // OCTREENODE_H
