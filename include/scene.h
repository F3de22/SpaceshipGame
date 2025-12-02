#pragma once
#include "utils/stb_image.h"
#include "renderer.h"
#include "camera.h"
#include "gameObject.h"
#include <vector>
#include <string>
#include <queue>

using namespace std;

namespace SpaceEngine
{
    class GameObject;
    
    class Scene
    {
        public:
            Scene();
            ~Scene();

            unsigned int LoadCubemap(vector<string> faces);
            void Init();
            void Render();
            void createGameObject();
            template<typename T>
            void addSceneComponet(T* sceneComponent);
            void gatherRenderables(std::vector<RenderObject>& worldRenderables, std::vector<UIRenderObject>& uiRenderables);
            void requestDestroy(GameObject* pGameObj);
            BaseCamera* getActiveCamera();
            void Update(float dt);
        private:
            void processDestroyQ();
            vector<GameObject*> gameObjects;
            std::queue<GameObject*> destroyQ;
            vector<BaseCamera*> cameras; 
            unsigned int cubemapTexture;
            vector<string> faces = {            //TODO: sistemare i path con i file giusti
                "assets/textures/skybox/right.jpg",
                "assets/textures/skybox/left.jpg",
                "assets/textures/skybox/top.jpg",
                "assets/textures/skybox/bottom.jpg",
                "assets/texture/skybox/front.jpg",
                //"assets/textures/skybox/back.jpg"
            };
    };
}