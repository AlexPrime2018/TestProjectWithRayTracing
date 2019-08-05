#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>

#include "scene.h"
#include "camera3d.h"

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QOpenGLShaderProgram *program, Camera3D *camera, Scene *scene, QWidget *parent = nullptr);
    ~Widget();
protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();
private:
    Scene *m_scene;
    QOpenGLShaderProgram *m_program;
    Camera3D *m_camera;
};

#endif // WIDGET_H
