#include "model3d.h"

#include <QOpenGLTexture>

Model3D::Model3D()
    : m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(nullptr), m_program(nullptr), m_functions(nullptr)
{
    m_scale = 1.0f;
    mode = GL_LINES;
    m_ID = "No ID";
}

Model3D::Model3D(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, const QVector<VertexData> &vertData, const QVector<int> &indexes, const QImage &texture, GLenum _mode)
    : m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(nullptr), m_program(program), m_functions(functions)
{
    m_scale = 1.0f;
    m_ID = "No ID";
    init(vertData, indexes, texture, _mode);
}

Model3D::~Model3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if(m_texture != nullptr)
        if(m_texture->isCreated())
            m_texture->destroy();
}

void Model3D::init(const QVector<VertexData> &vertData, const QVector<int> &indexes, const QImage &texture, GLenum _mode)
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if(m_texture != nullptr)
        if(m_texture->isCreated()) {
            delete m_texture;
            m_texture = nullptr;
        }

    mode = _mode;

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(int));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored());

    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);

    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Model3D::draw()
{
    if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated())
        return;

    m_texture->bind(0);
    m_program->bind();

    m_program->setUniformValue("u_texture", 0);

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_translate);
    m_modelMatrix.rotate(m_rotate);
    m_modelMatrix.scale(m_scale);

    m_program->setUniformValue("u_modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;

    int vertLoc = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertLoc);
    m_program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texLoc);
    m_program->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normLoc = m_program->attributeLocation("a_normal");
    m_program->enableAttributeArray(normLoc);
    m_program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexBuffer.bind();

    m_functions->glDrawElements(mode, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
    m_program->release();
}

void Model3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_translate);
    m_modelMatrix.rotate(m_rotate);
    m_modelMatrix.scale(m_scale);
}

void Model3D::rotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_translate);
    m_modelMatrix.rotate(m_rotate);
    m_modelMatrix.scale(m_scale);
}

void Model3D::rotateY(const QQuaternion &r)
{
    m_rotateY = r * m_rotateY;
    m_rotate = m_rotateY * m_rotateX;

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_translate);
    m_modelMatrix.rotate(m_rotate);
    m_modelMatrix.scale(m_scale);
}

void Model3D::translate(const QVector3D &t)
{
    m_translate += t;

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_translate);
    m_modelMatrix.rotate(m_rotate);
    m_modelMatrix.scale(m_scale);
}

void Model3D::setID(QString id)
{
    m_ID = id;
}

QMatrix4x4 Model3D::getModelMatrix()
{
    return m_modelMatrix;
}

QString Model3D::getID()
{
    return m_ID;
}
