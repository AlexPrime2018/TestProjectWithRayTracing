#include "scene.h"

Scene::Scene(QVector<Model3D *> models)
{
    m_models = models;
}

void Scene::draw(int w, int h)
{
    glViewport(0, 0, w, h);
    for (int i = 0; i < m_models.size(); i++)
        m_models[i]->draw();
}
