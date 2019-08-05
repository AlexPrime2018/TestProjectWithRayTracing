#include "loadobj.h"

LoadObj::LoadObj()
{
}

LoadObj::LoadObj(const QString &path)
{
    openOBJ(path);
}

LoadObj::~LoadObj()
{
    vertices.clear();
    texCoord.clear();
    normals.clear();
    m_faces.clear();
    indexes.clear();
    arrNormal.clear();
}

bool LoadObj::readLine(const QString &line)
{
    QString tmptestSpace = line;
    if (tmptestSpace.remove(" ").size() == 0)
        return true;

    if(line.at(0) == 'v' && line.at(1) == ' ') {
        if (!readVertices(line)) {
            qDebug() << "Wrong vertices line" << endl;
            return false;
        }
        else return true;
    }

    else if(line.at(0) == 'v' && line.at(1) == 't') {
         if (!readTexCoords(line)) {
             qDebug() << "Wrong texCoords line" << endl;
             return false;
         }
         else return true;
    }

    else if(line.at(0) == 'v' && line.at(1) == 'n') {
        if (!readNormals(line)) {
            qDebug() << "Wrong normals line" << endl;
            return false;
        }
        else return true;
    }

    else if(line.at(0) == 'f' && line.at(1) == ' ') {
        if(!readFaces(line)) {
            qDebug() << "Wrong faces line" << endl;
            return false;
        }
        else return true;
    }

    else if(line[0] == "mtllib") {
        return true;
    }

    else if(line.at(0) == 'g') {
        return true;
    }

    else if(line.at(0) == 'o') {
        return true;
    }

    else if(line.at(0) == 's') {
        return true;
    }

    else if(line.at(0) == '#') {
        qDebug() << line << endl;
        return true;
    }

    else {
       qDebug() << line << "An invalid string was submitted." << endl;
       return false;
    }
}

void LoadObj::openOBJ(const QString &path)
{
    objFile.setFileName(path);
    if (!objFile.exists()) {
        qDebug() << "File is not open!" << endl;
        return;
    }
    objFile.open(QIODevice::ReadOnly);
    QTextStream input(&objFile);

    while(!input.atEnd() && ok) {
        QString line = input.readLine();

        ok = readLine(line);
    }
}

QVector<VertexData> LoadObj::getVertexData()
{
    return vertexes;
}

QVector<int> LoadObj::getIndexes()
{
    return indexes;
}

QVector<QVector3D> LoadObj::getPoint()
{
    return vertices;
}

QVector<int> LoadObj::getFaces()
{
    return m_faces;
}

void LoadObj::calculNormal()
{
    for (int i = 0; i < vertices.size(); i++)
        arrNormal.insert(i, QVector3D(0.0f, 0.0f, 0.0f));

    for (int i = 0; i < m_faces.size(); i+=3)
    {
        int ia = m_faces.at(i + 0);
        int ib = m_faces.at(i + 1);
        int ic = m_faces.at(i + 2);

        ia -= 1;
        ib -= 1;
        ic -= 1;

        // TODO delet it
        const QVector3D a = vertices.at(ia);
        const QVector3D b = vertices.at(ib);
        const QVector3D c = vertices.at(ic);
        //

        const QVector3D edge1 = a - b;
        const QVector3D edge2 = b - c;
        const QVector3D normals = edge1.crossProduct(edge1, edge2);

        arrNormal[ia] += normals;
        arrNormal[ib] += normals;
        arrNormal[ic] += normals;
    }

    for (int i = 0; i < arrNormal.size(); i++)
        arrNormal[i].normalize();
}

bool LoadObj::readVertex(const QString &v, QVector3D &vertex)
{
    if (v.at(0) != 'v' && v.at(1) != ' ') return false;
    QStringList list = v.split(" ");
    bool ok;
    QVector<float> vv;
    for (int i = 0; i < list.size(); i++) {
        if(list[i] != "v" && list[i] != "") {
            vv.push_back(list[i].toFloat(&ok));
            if (!ok)
                return false;
        }
    }

    if (vv.size() == 3) {
        vertex[0] = vv[0];
        vertex[1] = vv[1];
        vertex[2] = vv[2];
        return true;
    }

    return false;
}

void LoadObj::testCalculNormal(const QVector<QVector3D> &points, const QVector<int> &faces, QVector<QVector3D> &normalsVertex)
{
    for (int i = 0; i < points.size(); i++)
        normalsVertex.insert(i, QVector3D(0.0f, 0.0f, 0.0f));

    for (int i = 0; i < faces.size(); i+=3)
    {
        int ia = faces.at(i + 0);
        int ib = faces.at(i + 1);
        int ic = faces.at(i + 2);

        ia -= 1;
        ib -= 1;
        ic -= 1;

        // TODO delet it
        const QVector3D a = points.at(ia);
        const QVector3D b = points.at(ib);
        const QVector3D c = points.at(ic);
        //

        const QVector3D edge1 = a - b;
        const QVector3D edge2 = b - c;
        const QVector3D normals = edge1.crossProduct(edge1, edge2);

        normalsVertex[ia] += normals;
        normalsVertex[ib] += normals;
        normalsVertex[ic] += normals;
    }

    for (int i = 0; i < normalsVertex.size(); i++)
        normalsVertex[i].normalize();
}

bool LoadObj::readVertices(const QString &vert)
{
    if (vert.at(0) != 'v' && vert.at(1) != ' ') return false;
    QStringList list = vert.split(" ");
    bool ok;
    QVector<float> v;
    for (int i = 0; i < list.size(); i++) {
        if(list[i] != "v" && list[i] != "") {
            v.push_back(list[i].toFloat(&ok));
            if (!ok)
                return false;
        }
    }

    if (v.size() == 3) {
        vertices.push_back(QVector3D(v[0], v[1], v[2]));
        return true;
    }

    return false;
}

bool LoadObj::readTexCoords(const QString &texCoords)
{
    if (texCoords.at(0) != 'v' && texCoords.at(1) != 't') return false;

    QStringList list = texCoords.split(" ");
    QVector<float> tc;
    for (int i = 0; i < list.size(); i++) {
        if(list[i] != "vt" && list[i] != "") {
            tc.push_back(list[i].toFloat());
        }
    }

    if (tc.size() == 2 || tc.size() == 3) {
        texCoord.push_back(QVector2D(tc[0], tc[1]));
        is_texCoord = true;
        return true;
    }
    else
        return false;
}

bool LoadObj::readNormals(const QString &normal)
{
    if (normal.at(0) != 'v' && normal.at(1) != 'n') return false;

    QStringList list = normal.split(" ");
    QVector<float> vn;
    for (int i = 0; i < list.size(); i++) {
        if(list[i] != "vn" && list[i] != "") {
            vn.push_back(list[i].toFloat());
        }
    }

    if (vn.size() == 3) {
        normals.push_back(QVector3D(vn[0], vn[1], vn[2]));
        is_normal = true;
        return true;
    }
    else {
        return false;
    }
}

bool LoadObj::readFaces(const QString &faces)
{
    if (faces.at(0) != 'f' && faces.at(1) != ' ') return false;

    if (!is_normal)
        calculNormal();

    QStringList face = faces.split(" ");
    for (int i = 1; i <= 3; i++) {
        QStringList vert = face[i].split("/");
        if (!is_texCoord && is_normal)
            vertexes.push_back(VertexData(vertices[vert[0].toLong() - 1], QVector2D(1, 1), normals[vert[2].toLong() - 1]));
        if (!is_normal && is_texCoord)
            vertexes.push_back(VertexData(vertices[vert[0].toLong() - 1], texCoord[vert[1].toLong() - 1], arrNormal[vert[2].toLong() - 1]));
        if (!is_texCoord && !is_normal)
            vertexes.push_back(VertexData(vertices[vert[0].toLong() - 1], QVector2D(1, 1), QVector3D(1, 1, 1)));
        if (is_texCoord && is_normal)
            vertexes.push_back(VertexData(vertices[vert[0].toLong() - 1], texCoord[vert[1].toLong() - 1], normals[vert[2].toLong() - 1]));
        indexes.append(indexes.size());
        m_faces.push_back(vert[0].toLong());
    }

    return true;
}
