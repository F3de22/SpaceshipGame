#include "gameObject.h"
#include "scene.h"
#include "log.h"

namespace SpaceEngine
{
    template<typename T>
    T* GameObject::getComponent()
    {
        if constexpr (std::is_same_v<T, Mesh>)
        {
            return m_pMesh;
        }
        else if constexpr (std::is_same_v<T, std::vector<Transform*>>)
        {
            return &m_vecTransform;
        }

        SPACE_ENGINE_ERROR("Component not found");
        return nullptr;
    }

    template<typename T>
    void GameObject::addComponent(T componet)
    {
        if(!componet)
        {
            SPACE_ENGINE_ERROR("The passed component is null");
            return;
        }
        if constexpr (std::is_same_v<T, Mesh*>)
        {
            m_pMesh = componet;
        }
        else if constexpr (std::is_same_v<T, Transform*>)
        {
            m_vecTransform.push_back(componet);
        }
    }

    void GameObject::destroy()
    {
        scene->requestDestroy(this);
    }

    int GameObject::getNumInstances()
    {
        return m_numInstances;
    }
}