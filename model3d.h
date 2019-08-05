#ifndef MODEL3D_H
#define MODEL3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include "vertexdata.h"

class QOpenGLTexture;

class Model3D
{
public:
    Model3D();
    Model3D(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, const QVector<VertexData> &vertData, const QVector<int> &indexes, const QImage &texture, GLenum _mode);
    ~Model3D();

    void init(const QVector<VertexData> &vertData, const QVector<int> &indexes, const QImage &texture, GLenum _mode);

    void draw();

    void rotate(const QQuaternion &r);
    void rotateX(const QQuaternion &r);
    void rotateY(const QQuaternion &r);
    void translate(const QVector3D &t);

    void setID(QString id);

    QMatrix4x4 getModelMatrix();
    QString getID();
private:
    GLenum mode;

    QString m_ID;

    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLTexture *m_texture;
    QMatrix4x4 m_modelMatrix;

    QVector3D m_translate;
    float m_scale;
    QQuaternion m_rotate;
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;

    QOpenGLShaderProgram *m_program;
    QOpenGLFunctions *m_functions;
};

#endif // MODEL3D_H
