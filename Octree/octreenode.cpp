#include "octreenode.h"
#include <QDebug>
#include "triangle3d.h"

unsigned int OctreeNode::LEVEL_MAX = 4;
unsigned int OctreeNode::LIMIT_MAX = 10;

OctreeNode::OctreeNode(const QVector3D &center, const QVector3D &size, const unsigned int& level, OctreeNode* parent) :
    m_center(center), m_size(size), m_level(level), m_parent(parent)
{
    float x = m_center.x();
    float y = m_center.y();
    float z = m_center.z();

    m_limitReached = true;

    m_childCenter[0].setX(x - m_size.x() / 2.0f);
    m_childCenter[0].setY(y + m_size.y() / 2.0f);
    m_childCenter[0].setZ(z - m_size.z() / 2.0f);

    m_childCenter[1].setX(x + m_size.x() / 2.0f);
    m_childCenter[1].setY(y + m_size.y() / 2.0f);
    m_childCenter[1].setZ(z - m_size.z() / 2.0f);

    m_childCenter[2].setX(x - m_size.x() / 2.0f);
    m_childCenter[2].setY(y - m_size.y() / 2.0f);
    m_childCenter[2].setZ(z - m_size.z() / 2.0f);

    m_childCenter[3].setX(x + m_size.x() / 2.0f);
    m_childCenter[3].setY(y - m_size.y() / 2.0f);
    m_childCenter[3].setZ(z - m_size.z() / 2.0f);

    m_childCenter[4].setX(x - m_size.x() / 2.0f);
    m_childCenter[4].setY(y + m_size.y() / 2.0f);
    m_childCenter[4].setZ(z + m_size.z() / 2.0f);

    m_childCenter[5].setX(x + m_size.x() / 2.0f);
    m_childCenter[5].setY(y + m_size.y() / 2.0f);
    m_childCenter[5].setZ(z + m_size.z() / 2.0f);

    m_childCenter[6].setX(x - m_size.x() / 2.0f);
    m_childCenter[6].setY(y - m_size.y() / 2.0f);
    m_childCenter[6].setZ(z + m_size.z() / 2.0f);

    m_childCenter[7].setX(x + m_size.x() / 2.0f);
    m_childCenter[7].setY(y - m_size.y() / 2.0f);
    m_childCenter[7].setZ(z + m_size.z() / 2.0f);

    for (int indexChild = 0; indexChild < 8; indexChild++)
        m_child[indexChild] = nullptr;
}

OctreeNode::~OctreeNode()
{
    for (int indexChild = 0; indexChild < 8; indexChild++)
        if (m_child[indexChild])
            delete m_child[indexChild];
}

bool OctreeNode::intersection(const Ray &ray, Triangle3D *closestTriangle)
{
    bool hasIntersection = false;
    QVector3D half_size = m_size / 2.0f;

    if (!HelperFunctions::rayIntersectionAABB(ray, m_center, m_size))
        return false;

    if (HelperFunctions::rayIntersectionAABB(ray, m_center, m_size))
    {
        if (!m_limitReached || m_level == LEVEL_MAX)
        {
            for (Triangle3D *tri : m_trianglesThisNode)
            {
                if (tri->intersection(ray, IntersectionPoint))
                {
                    closestTriangle = tri;
                    hasIntersection = true;
                }
            }
        }
        else
        {
            std::multimap<float, int> cubeOrder;

            for (int i = 0; i < 8; i++)
            {
                float tmpDist = std::numeric_limits<float>::max();
                if (m_child[i] && HelperFunctions::rayIntersectionAABB(ray, m_childCenter[i], half_size))
                {
                    cubeOrder.insert(std::pair<float, int>(tmpDist, i));
                }
            }

            for (std::pair<float, int> order : cubeOrder)
            {
                int childIndex = order.second;

                if (m_child[childIndex] && m_child[childIndex]->intersection(ray, closestTriangle))
                {
                    hasIntersection = true;
                }
            }
        }
    }

    return hasIntersection;
}

void OctreeNode::addTriangle(Triangle3D *triangles)
{
    QVector3D half_size = m_size / 2.0f;

    if (m_level == LEVEL_MAX)
    {
        m_trianglesThisNode.push_back(triangles);

        if (m_trianglesThisNode.size() >= LIMIT_MAX && m_level < LEVEL_MAX)
        {
            m_limitReached = true;
            for (Triangle3D *tri : m_trianglesThisNode)
            {
                QVector3D p0 = tri->getP0();
                QVector3D p1 = tri->getP1();
                QVector3D p2 = tri->getP2();

                for (int i = 0; i < 8; i++)
                {
                    if (HelperFunctions::triangleInsideAABB(p0, p1, p2, m_childCenter[i], half_size))
                    {
                        if (!m_child[i])
                            m_child[i] = new OctreeNode(m_childCenter[i], half_size, m_level + 1, this);

                        m_child[i]->addTriangle(tri);
                    }
                }
            }
            m_trianglesThisNode.clear();
        }
    }
    else
    {
        QVector3D p0 = triangles->getP0();
        QVector3D p1 = triangles->getP1();
        QVector3D p2 = triangles->getP2();

        for (int i = 0; i < 8; i++)
        {
            if (HelperFunctions::triangleInsideAABB(p0, p1, p2, m_childCenter[i], half_size))
            {
                if (!m_child[i])
                    m_child[i] = new OctreeNode(m_childCenter[i], half_size, m_level + 1, this);

                m_child[i]->addTriangle(triangles);
            }
        }
    }
}
