#include "gameObject.h"
#include "scene.h"
#include "collisionDetection.h"
#include "log.h"

namespace SpaceEngine
{
    GameObject::GameObject(const GameObject& other)
    {
        scene = other.scene;
        m_pMesh = other.m_pMesh;
        m_layer = other.m_layer;
        m_pTransform = new Transform(*other.m_pTransform);
    }

    Layers GameObject::getLayer()
    {
        return m_layer;
    }
    void GameObject::destroy()
    {
        pendingDestroy = true;
        scene->requestDestroy(this);
    }

    void GameObject::update(float dt)
    {

    }

    void GameObject::fixedUpdate(float fixed_dt)
    {

    }

    void GameObject::onCollisionEnter(Collider* col)
    {

    }
}