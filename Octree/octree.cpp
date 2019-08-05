#include "octree.h"
#include "octreenode.h"
#include "triangle3d.h"

Octree::Octree(const QVector<QVector3D> &points, const QVector<int> &faces) :
    m_points(points), m_faces(faces)
{
    setRootCenterAndSize();
    makeTriangle();
}

bool Octree::intersection(const Ray &ray)
{
    return m_root->intersection(ray, &closestTriangl);
}

void Octree::addTriangle(Triangle3D *tri)
{
    m_root->addTriangle(tri);
}

void Octree::clear()
{
    delete m_root;
    m_root = nullptr;
    setRootCenterAndSize();
}

Triangle3D Octree::getTriangle()
{
    return closestTriangl;
}

void Octree::makeTriangle()
{
    for (int i = 0; i < m_faces.size(); i+=3)
    {
        int ia = m_faces.at(i + 0) - 1;
        int ib = m_faces.at(i + 1) - 1;
        int ic = m_faces.at(i + 2) - 1;

        QVector3D a = m_points.at(ia);
        QVector3D b = m_points.at(ib);
        QVector3D c = m_points.at(ic);

        addTriangle(new Triangle3D(a, b, c));
    }
}

void Octree::setRootCenterAndSize()
{
    float inf = std::numeric_limits<float>::max();

    float xmin = inf, ymin = inf, zmin = inf;
    float xmax = -inf, ymax = -inf, zmax = -inf;

    for (int i = 0; i < m_points.size(); i++)
    {
        QVector3D p = m_points.at(i);

        if (p.x() < xmin)
            xmin = p.x();
        if (p.y() < ymin)
            ymin = p.y();
        if (p.z() < zmin)
            zmin = p.z();

        if (p.x() > xmax)
            xmax = p.x();
        if (p.y() > ymax)
            ymax = p.y();
        if (p.z() > zmax)
            zmax = p.z();
    }
    QVector3D center = QVector3D((xmax + xmin) / 2.0f, (ymax + ymin) / 2.0f, (zmax + zmin) / 2.0f);
    QVector3D size = QVector3D((xmax - xmin) / 2.0f, (ymax - ymin) / 2.0f, (zmax - zmin) / 2.0f);

    m_root = new OctreeNode(center, size, 0);
}
