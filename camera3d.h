#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <QMatrix4x4>
#include <QVector2D>

class Camera3D
{
public:
    Camera3D();
    ~Camera3D();

    QMatrix4x4 getViewMatrix();

    void translate(const QVector3D &t);
    void rotate(const QQuaternion &r);
    void rotate(float rotateAngle, const QVector3D &rotateAxis);
    void rotateX(const QQuaternion &r);
    void rotateY(const QQuaternion &r);
    void scale(const float &s);

    void setGlobalTransform(const QMatrix4x4 &g);
    void setPerspective(float fov, int w, int h, float zNear, float zFar);

    const QMatrix4x4 &getProjectionMatrix() const;
    const float &getZFar() const;

    void updateViewMatrix();

private:
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_globalTransform;
    QMatrix4x4 m_projectionMatrix;

    QQuaternion m_rotate;
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;

    QVector3D m_translate;
    float m_scale;
    float m_zFar;
};

#endif // CAMERA3D_H
