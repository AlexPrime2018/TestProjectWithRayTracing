#ifndef LOADOBJ_H
#define LOADOBJ_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QDebug>
#include <QFile>
#include <QString>
#include "vertexdata.h"

class LoadObj
{
public:
    LoadObj();
    LoadObj(const QString &path);
    ~LoadObj();

    bool readLine(const QString &line);
    void openOBJ(const QString &path);

    QVector<VertexData> getVertexData();
    QVector<int> getIndexes();
    QVector<QVector3D> getPoint();
    QVector<int> getFaces();
private:
    friend class Tests;
    static bool readVertex(const QString &s, QVector3D &vertex);
    static void testCalculNormal(const QVector<QVector3D> &points, const QVector<int> &face, QVector<QVector3D> &normalsVertex);

    void calculNormal();

    bool readVertices(const QString &vert);
    bool readTexCoords(const QString &texCoords);
    bool readNormals(const QString &normal);
    bool readFaces(const QString &faces);

    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoord;
    QVector<QVector3D> normals;
    QVector<int> m_faces;

    QVector<VertexData> vertexes;
    QVector<int> indexes;

    QVector<QVector3D> arrNormal;

    bool is_texCoord = false, is_normal = false;
    bool ok = true;

    QFile objFile;
};

#endif // LOADOBJ_H
