#include "sceneManager.h"

namespace SpaceEngine
{
    std::vector<Scene*> SceneManager::m_vecScenes;
    Scene* SceneManager::m_currScene = nullptr;

    void SceneManager::Shutdown()
    {
        for(Scene* pScene : m_vecScenes)
        {
            delete pScene;
        }
    }

    void SceneManager::Initialize()
    {

    }

    void SceneManager::Update(float dt)
    {
        for(Scene* pScene : m_vecScenes)
        {
            if(pScene->isActive())
            {
                pScene->Update(dt);
            }
        }
    }

    void SceneManager::GatherRenderables(std::vector<RenderObject>& worldRenderables, 
                std::vector<UIRenderObject>& uiRenderables)
    {
        worldRenderables.clear();
        uiRenderables.clear();

        for(Scene* pScene: m_vecScenes)
        {
            if(pScene->isActive())
            {
                pScene->gatherRenderables(worldRenderables, uiRenderables);
            }
        }
    }

    void SceneManager::LoadScene(Scene* pScene)
    {
        pScene->OnLoad();
        m_vecScenes.push_back(pScene);
    }

    void SceneManager::UnloadScene(Scene* pScene)
    {
        //make a queue to deallocate the scene
    }
 
    void SceneManager::SwitchScene(const std::string & name)
    {
        for(Scene* pScene : m_vecScenes)
        {
            if((!pScene->isActive()) && pScene->getNameScene() == name)
            {
                m_currScene->setActive(false);
                m_currScene = pScene;
                m_currScene->setActive(true);
            }
        }
    }



}