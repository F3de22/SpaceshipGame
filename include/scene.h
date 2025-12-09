#pragma once
#include "utils/stb_image.h"
#include "renderer.h"
#include "camera.h"
#include "gameObject.h"
#include "collisionDetection.h"
#include "log.h"
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
            Scene() = default;
            ~Scene() = default;

            unsigned int LoadCubemap(vector<string> faces);
            void Init();
            void createGameObject();

            template<typename T>
            void addSceneComponent(T sceneComponent)
            {
                using PureT = std::remove_pointer_t<T>;

                if(sceneComponent == nullptr)
                {
                    SPACE_ENGINE_ERROR("The passed component is null");
                    return;
                }
                if constexpr (std::is_base_of<GameObject, PureT>::value)
                {
                    gameObjects.push_back(sceneComponent);
                    Collider * pCol = sceneComponent->getComponent<Collider>();
                    
                    if(pCol != nullptr) 
                    {
                        pPhyManager->AddCollider(pCol); 
                    }
                    
                    return;
                }
                else if constexpr (std::is_base_of<BaseCamera, PureT>::value)
                {
                    cameras.push_back(sceneComponent);
                    return;
                }
            
                SPACE_ENGINE_ERROR("Component not valid!");
            }

            void gatherColliderables();
            void gatherRenderables(std::vector<RenderObject>& worldRenderables, std::vector<UIRenderObject>& uiRenderables);
            void requestDestroy(GameObject* pGameObj);
            void requestInstatiate(GameObject* pGameObj);
            void requestInstatiate(GameObject* pGameObj, float time);
            void requestInstatiate(GameObject* pGameObj, Vector3 wPos);
            void requestInstatiate(GameObject* pGameObj, float time, Vector3 wPos);
            BaseCamera* getActiveCamera();
            void Update(float dt);
        private:
            PhysicsManager* pPhyManager = nullptr;
            void processDestroyQ();
            void processInstantiateQ(float dt);
            struct SpawnRequest
            {
                float timeRemaining = 0.f;
                GameObject* prefab = nullptr;
                bool overrideWorldPos = false;
                Vector3 wPos;
            };
        
            GameObject* instatiate(const SpawnRequest& sr);
            //may be is useful a table with duoble link list to keep the gameObjects
            //for avoing a complex gathering
            vector<GameObject*> gameObjects;
            std::queue<GameObject*> destroyQ;
            std::list<SpawnRequest> spawnQ;
            //cameras[0] is always the active camera
            vector<BaseCamera*> cameras; 
            unsigned int cubemapTexture;
            vector<string> faces = {            //TODO: sistemare i path con i file giusti
                "assets/textures/skybox/right.jpg",
                "assets/textures/skybox/left.jpg",
                "assets/textures/skybox/top.jpg",
                "assets/textures/skybox/bottom.jpg",
                "assets/texture/skybox/front.jpg",
                "assets/textures/skybox/back.jpg"
            };
    };
}