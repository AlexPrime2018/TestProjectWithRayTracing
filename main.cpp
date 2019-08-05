#include "widget.h"
#include "model3d.h"
#include "scene.h"
#include "camera3d.h"
#include "loadobj.h"
#include "navlog.h"
#include "octree.h"
#include "tests.h"
#include "timer.h"

#include <QApplication>
#include <QOffscreenSurface>
#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QWheelEvent>
#include <QTest>

void runTests();
void initCube(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, float width, float height, float depth, const QImage &texture, const QVector3D &position);
void initShaders(QOpenGLShaderProgram &program);
void CreateModel(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, QVector<Octree *> &octree, const QString &pathOBJ, const QString &pathTexture, const QString &modelID, const QVector3D &position = QVector3D(0.0f, 0.0f, 0.0f));
void drawLine(QVector3D startPos, QVector3D endPos, QVector<QVector3D> &vertLine, QVector<int> &indices);
void drawPlane(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, int windowWidth, float windowHeigh, float step);
void drawRay(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, const QVector3D &orig, const QVector3D &dir);

int main(int argc, char *argv[])
{
    // Получение Context
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);

    QOpenGLContext *glContextGlobal = QOpenGLContext::globalShareContext();

    QOffscreenSurface *glSurface = new QOffscreenSurface();
    glSurface->create();
    QOpenGLContext *glContext = new QOpenGLContext();
    glContext->setShareContext(glContextGlobal);
    glContext->create();

    glContext->makeCurrent(glSurface);

    qDebug() << "OpenGL version" << glContext->format().version().first << glContext->format().version().second << endl;
    //

    runTests();

    QOpenGLShaderProgram m_program;

    initShaders(m_program);

    // Создание сцены
    Camera3D *m_camera = new Camera3D();
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
    QVector<Model3D *> m_models;
    QVector<Octree *> m_octree;

    //initCube(m_program, glContext->functions(), m_models, 1.0f, 1.0f, 1.0f, QImage(":/brik.jpg"), QVector3D(0.0f, 0.0f, 0.0f));

    drawPlane(m_program, glContext->functions(), m_models, 6.0f, 6.0f, 0.25f);
    CreateModel(m_program, glContext->functions(), m_models, m_octree, ":/obj.obj", ":/obj.jpg", "1");
    //CreateModel(m_program, glContext->functions(), m_models, m_octree, ":/obj.obj", ":/obj.jpg", "1", QVector3D(3.0f, 0.0f, 0.0f));
    //CreateModel(m_program, glContext->functions(), m_models, m_octree, ":/obj.obj", ":/obj.jpg", "1", QVector3D(-3.0f, 0.0f, 0.0f));
    Scene *m_scene = new Scene(m_models);

    Widget w(&m_program, m_camera, m_scene);

    NavLog nav(&w, m_camera, m_octree);
    w.installEventFilter(&nav);

    w.show();

    return a.exec();
}

void runTests()
{
    Tests tests;
    QTest::qExec(&tests);
}

void initCube(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, float width, float height, float depth, const QImage &texture, const QVector3D &position)
{
    float width_div_2 = width / 2.0f;
    float height_div_2 = height / 2.0f;
    float depth_div_2 = depth / 2.0f;
    QVector<VertexData> vertexes;

    vertexes.append(VertexData(QVector3D(-width_div_2,height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0,0.0,1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-height_div_2, depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0,0.0,1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0,0.0,1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-height_div_2, depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0,0.0,1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2,height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-height_div_2, depth_div_2), QVector2D(0.0, 0.0), QVector3D(1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,height_div_2, -depth_div_2), QVector2D(1.0, 1.0), QVector3D(1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(1.0,0.0,0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2,height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0,1.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0,1.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0,1.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0,1.0,0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, height_div_2, -depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0,0.0,-1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0,0.0,-1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,height_div_2, -depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0,0.0,-1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0,0.0,-1.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2,height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(-1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(-1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(-1.0,0.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-height_div_2,-depth_div_2), QVector2D(1.0, 0.0), QVector3D(-1.0,0.0,0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2,-height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0,-1.0,0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2,-height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0,-1.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0,-1.0,0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2,-height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0,-1.0,0.0)));

    QVector<int> indexes;
    for (int i = 0; i < 24; i+=4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);

        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    Model3D *mo = new Model3D(&program, function, vertexes, indexes, texture, GL_TRIANGLES);
    mo->setID("1");
    mo->translate(position);
    models.append(mo);
    //models[models.size() - 1]->translate(QVector3D(0.0f, 1.0f, 0.0f));
    models[models.size() - 1]->rotate(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
}

void initShaders(QOpenGLShaderProgram &program)
{
    if(!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
        exit(-1);
    if(!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
        exit(-1);
    if(!program.link())
        exit(-1);
}

void CreateModel(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, QVector<Octree *> &octree, const QString &pathOBJ, const QString &pathTexture, const QString &modelID, const QVector3D &position)
{
    LoadObj lo;
    lo.openOBJ(pathOBJ);
    Model3D *mo = new Model3D(&program, function, lo.getVertexData(), lo.getIndexes(), QImage(pathTexture), GL_TRIANGLES);

    mo->setID(modelID);
    mo->translate(position);

    QMatrix4x4 matrix = mo->getModelMatrix();
    QVector<QVector3D> points;
    if (matrix(0, 3) > 0.0f || matrix(1, 3) > 0.0f || matrix(2, 3) > 0.0f)
        for (int i = 0; i < lo.getPoint().size(); i++)
            points.insert(i, matrix * lo.getPoint().at(i));

    ClockTimer timer;
    if (points.isEmpty())
        octree.push_back(new Octree(lo.getPoint(), lo.getFaces()));
    else
        octree.push_back(new Octree(points, lo.getFaces()));
    qDebug() << timer.get_elapsed() << " ms" << endl;

    models.push_back(mo);
}

void drawLine(QVector3D startPos, QVector3D endPos, QVector<QVector3D> &vertLine, QVector<int> &indices)
{
    vertLine.push_back(startPos);
    vertLine.push_back(endPos);

    indices.push_back(indices.size());
    indices.push_back(indices.size());
}

void drawPlane(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, int windowWidth, float windowHeigh, float step)
{
    QVector<VertexData> vd;

    QVector<QVector3D> vertLine;
    QVector<int> indices;

    for (float x = 0.0f; x <= windowWidth; x += step) {
        drawLine(QVector3D(x, 0.0f, 0.0f), QVector3D(x, windowHeigh, 0.0f), vertLine, indices);
    }

    for (float y = 0.0f; y <= windowHeigh; y += step) {
        drawLine(QVector3D(0.0f, y, 0.0f), QVector3D(windowWidth, y, 0.0f), vertLine, indices);
    }

    for (int i = 0; i < vertLine.size(); i++)
        vd.push_back(VertexData(vertLine[i], QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 0.0)));

    models.append(new Model3D(&program, function, vd, indices, QImage(":/obj.jpg"), GL_LINES));
    models[models.size() - 1]->translate(QVector3D(windowWidth / -2.0f, -0.8f, windowHeigh / -2.0f));
    models[models.size() - 1]->rotate(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
}

void drawRay(QOpenGLShaderProgram &program, QOpenGLFunctions *function, QVector<Model3D *> &models, const QVector3D &orig, const QVector3D &dir)
{
    QVector<VertexData> vd;

    QVector<QVector3D> vertLine;
    QVector<int> indices;

    drawLine(orig, dir, vertLine, indices);

    for (int i = 0; i < vertLine.size(); i++)
        vd.push_back(VertexData(vertLine[i], QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 0.0)));

    models.append(new Model3D(&program, function, vd, indices, QImage(":/obj.jpg"), GL_TRIANGLES));
}
