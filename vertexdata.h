#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <QVector2D>
#include <QVector3D>

struct VertexData {
    VertexData()
    {
    }

    VertexData(QVector3D c, QVector2D t, QVector3D n) :
        coords(c), texCoords(t), normals(n)
    {
    }

    QVector3D coords;
    QVector2D texCoords;
    QVector3D normals;
};

#endif // VERTEXDATA_H
