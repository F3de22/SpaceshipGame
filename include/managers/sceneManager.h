#pragma once
#include "../scene.h"

namespace SpaceEngine
{
    class SceneManager
    {
        public:
            SceneManager() = default;
            ~SceneManager() = default;
            void Initialize();
            void Shutdown();
            void Update(float dt);
            void GatherRenderables(std::vector<RenderObject>& worldRenderables, 
                std::vector<UIRenderObject>& uiRenderables);
            static void LoadScene(Scene* pScene);
            static void UnloadScene(Scene* pScene);
            static void SwitchScene(const std::string & name);
            


        private:
            static std::vector<Scene*> m_vecScenes;
            static Scene* m_currScene;

    };
}