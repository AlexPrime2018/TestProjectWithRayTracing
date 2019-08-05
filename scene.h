#ifndef SCENE_H
#define SCENE_H

#include "model3d.h"

#include <QVector>

class Scene
{
public:
    Scene(QVector<Model3D *> models);

    void draw(int w, int h);
private:
    QVector<Model3D *> m_models;
};

#endif // SCENE_H
