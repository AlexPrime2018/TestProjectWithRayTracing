#include "camera3d.h"
#include <qmath.h>

Camera3D::Camera3D()
{
    m_scale = 1.0f;
    m_globalTransform.setToIdentity();
}

QMatrix4x4 Camera3D::getViewMatrix()
{
    updateViewMatrix();

    return m_viewMatrix;
}

void Camera3D::translate(const QVector3D &t)
{
    m_translate += t;

    updateViewMatrix();
}

void Camera3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;

    updateViewMatrix();
}

void Camera3D::rotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;

    updateViewMatrix();
}

void Camera3D::rotateY(const QQuaternion &r)
{
    m_rotateY = r * m_rotateY;
    m_rotate = m_rotateY * m_rotateX;

    updateViewMatrix();
}

void Camera3D::scale(const float &s)
{
    m_scale = s * m_scale;

    updateViewMatrix();
}

void Camera3D::setPerspective(float fov, int w, int h, float zNear, float zFar)
{
    m_zFar = zFar;
    float AspectRatio = w / static_cast<float>(h ? h : 1);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(fov, AspectRatio, zNear, zFar);
}

void Camera3D::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;

    updateViewMatrix();
}

const QMatrix4x4 &Camera3D::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

const float &Camera3D::getZFar() const
{
    return m_zFar;
}

void Camera3D::updateViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.translate(m_translate);
    m_viewMatrix.rotate(m_rotate);
    m_viewMatrix.scale(m_scale);
    m_viewMatrix = m_viewMatrix * m_globalTransform.inverted();
}
