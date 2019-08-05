#include "widget.h"

Widget::Widget(QOpenGLShaderProgram *program, Camera3D *camera, Scene *scene, QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_scene = scene;
    m_camera = camera;
    m_program = program;
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Widget::resizeGL(int w, int h)
{
    m_camera->setPerspective(45.0f, w, h, 0.1f, 1000.0f);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_program->setUniformValue("u_projectionMatrix", m_camera->getProjectionMatrix());
    m_program->setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_program->setUniformValue("u_lightPower", 1.0f);
    m_program->setUniformValue("u_viewMatrix", m_camera->getViewMatrix());

    m_scene->draw(width(), height());

    m_program->release();
}
