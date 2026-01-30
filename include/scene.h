#pragma once

#include "utils/stb_image.h"
#include "renderer.h"
#include "camera.h"
#include "collisionDetection.h"
#include "light.h"
#include "log.h"
#include "shader.h"
#include "managers/audioManager.h"
#include "bullet.h"

#include "sceneManager.h"
#include "pauseScene.h"
#include <vector>
#include <string>
#include <queue>
#include <stack>

using namespace std;

namespace SpaceEngine
{
    class GameObject;
    class PlayerShip;
    class Asteroid;
    class EnemyShip;
    
    class Scene
    {
        public:
            Scene(PhysicsManager* pPhyManager):pPhyManager(pPhyManager)
            {
                //add a constructor where you can pass the path of skybox
                pSkybox = nullptr;
            };
            virtual ~Scene() = default;
            
            virtual void OnLoad()
            {
                SPACE_ENGINE_INFO("On load scene");
            }

            virtual void OnSwitch()
            {
                SPACE_ENGINE_INFO("On switch on the scene");
            }

            virtual void OnUnload()
            {
                SPACE_ENGINE_INFO("On unload scene");
            }

            void Init();

            inline void setActive(bool flag){active = flag;}
            inline bool isActive() const{return active;}
            inline void setNameScene(const std::string& name){this->name = name;}
            inline std::string getNameScene() const{return name;}
            inline Skybox* getSkybox() const{return pSkybox;}
            void notifyChangeRes();

            void setAudioManager(AudioManager* am) { pAudioManager = am; }
            AudioManager* getAudioManager() { return pAudioManager; }
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
                else if constexpr (std::is_base_of<Light, PureT>::value)
                {
                    lights.push_back(sceneComponent);
                    return;
                }
                else if constexpr (std::is_base_of<UILayout, PureT>::value)
                {
                    m_vecUILayouts.push_back(sceneComponent);
                    return;
                }
                SPACE_ENGINE_ERROR("Component not valid!");
            }

            void gatherRenderables(std::vector<RenderObject>& worldRenderables, std::vector<UIRenderObject>& uiRenderables, std::vector<TextRenderObject>& textRenderables);
            void requestDestroy(GameObject* pGameObj);
            
            template <typename T>
            T* requestInstantiate(const T* prefab, float time = 0.f)
            {
                return requestInstantiateImpl(prefab, time, false, {});
            }

            template <typename T>
            T* requestInstantiate(const T* prefab, Vector3 wPos)
            {
                return requestInstantiateImpl(prefab, 0.0f, true, wPos);
            }

            template <typename T>
            T* requestInstantiate(const T* prefab, float time, Vector3 wPos)
            {
                return requestInstantiateImpl(prefab, time, true, wPos);
            }

            BaseCamera* getActiveCamera() const;
            std::vector<Light*>* getLights() const; 
            void Update(float dt);

        private:
            struct SpawnRequest
            {
                float timeRemaining = 0.f;
                GameObject* prefab = nullptr;
                bool overrideWorldPos = false;
                Vector3 wPos;
            };            

            PhysicsManager* pPhyManager = nullptr;
            virtual void UpdateScene(float dt){}
            void processDestroyQ();
            void processInstantiateQ(float dt);
            inline void enqueueSpawn(SpawnRequest&& sr){spawnQ.push_back(std::move(sr));}
            template <typename T>
            T* requestInstantiateImpl(const T* prefab,
                                        float time,
                                        bool overrideWorldPos,
                                        const Vector3& wPos)
            {
                static_assert(std::is_base_of_v<GameObject, T>,
                              "T must derive from GameObject");
                
                SpawnRequest sr;
                T* pObj = new T(*prefab);
                sr.prefab = pObj;  
                sr.timeRemaining = time;
                sr.overrideWorldPos = overrideWorldPos;
                sr.wPos = wPos;
                  
                enqueueSpawn(std::move(sr));

                return pObj;
            }

            //Don't use it to instantiate GameObjects directly instead use RequestInstatiate
            GameObject* instantiate(const SpawnRequest& sr);
            //may be is useful a table with duoble link list to keep the gameObjects
            //for avoing a complex gathering
            //vector<GameObject*> gameObjects;
            std::queue<GameObject*> destroyQ;
            std::list<SpawnRequest> spawnQ;
            std::vector<Light*> lights;
            //cameras[0] is always the active camera
            vector<BaseCamera*> cameras;
            Skybox* pSkybox = nullptr;
            AudioManager* pAudioManager = nullptr;
            //scene property
            bool active = true;
        protected:
            vector<GameObject*> gameObjects;
            std::string name;
            std::vector<UILayout*> m_vecUILayouts;
    };

    class SpawnerSys
    {
        public:
            void handlerSpawn()
            {
                switch(m_stage.eStage)
                {
                    case SPAWN_ASTEROID_EASY:
                        if(m_stage.budget == 0)
                        {
                            m_stage.eStage = SPAWN_ASTEROID_MED;
                            m_stage.minSpawn = 1; 
                            m_stage.maxSpawn = 2;
                            m_stage.budget = BudgetAsteroridM;
                            m_stage.weights[0] = 0.3f;
                            m_stage.weights[1] = 0.7f;
                            m_stage.weights[2] = 0.f;
                            m_stage.spawnInterval = TimeAsterorid * TimeAsteroridXM; 
                        }
                        break;
                    case SPAWN_ASTEROID_MED:
                        if(m_stage.budget == 0)
                        {
                            m_stage.eStage = SPAWN_ASTEROID_HARD;
                            m_stage.minSpawn = 1; 
                            m_stage.maxSpawn = 3;
                            m_stage.budget = BudgetAsteroridH;
                            m_stage.weights[0] = 0.1f;
                            m_stage.weights[1] = 0.3f;
                            m_stage.weights[2] = 0.6f;
                            m_stage.spawnInterval = TimeAsterorid * TimeAsteroridXH; 
                        }
                        break;
                    case SPAWN_ASTEROID_HARD:
                        if(m_stage.budget == 0)
                        {
                            m_stage.eStage = SPAWN_ENEMY_EASY;
                            clearSpace(); 
                        }

                        break;
                    case SPAWN_ENEMY_EASY:
                        if(m_stage.budget == 0)
                        {
                            m_state = SPAWN_ENEMY_MED; 
                        }
                        break;
                    case SPAWN_ENEMY_MED:
                        if(m_stage.budget == 0)
                        {
                            m_state = SPAWN_ENEMY_HARD; 
                        }
                        break;
                    case SPAWN_ENEMY_HARD:
                        if(m_stage.budget == 0)
                        {
                            m_lastStage = true;
                            m_state = SPAWN_ASTEROID_HARD; 
                        }
                        break;
                }

                spawnLogic();
            } 

            void clearSpace()
            {
                for(int i = 0; i < SlotDim; i++)
                    m_space[i] = ESlot::FREE;
            }


        private:
            enum ESpawnState
            {
                SPAWN_ASTEROID_EASY,
                SPAWN_ASTEROID_MED,
                SPAWN_ASTEROID_HARD,
                SPAWN_ENEMY_EASY,
                SPAWN_ENEMY_MED,
                SPAWN_ENEMY_HARD,
                SPAWN_MIX
            };

            enum ESlot
            {
                FREE,
                ASTEROID,
                ENEMY
            };

            struct Stage
            {
                int weights[3];
                ESpawnState eStage;
                uint32_t budget;
                uint32_t minSpawn;
                uint32_t maxSpawn;
                float spawnInterval;
            };


            ESpawnState m_state = ESpawnState::SPAWN_ASTEROID_EASY;
            static constexpr int32_t BudgetAsteroridE = 20;
            static constexpr int32_t BudgetAsteroridM = 30;
            static constexpr int32_t BudgetAsteroridH = 10;
            static constexpr int32_t BudgetEnemyE = 20;
            static constexpr int32_t BudgetEnemyM = 30;
            static constexpr int32_t BudgetEnemyH = 10;
            static constexpr int32_t SlotDim = 3;
            static constexpr float StartOffsetSpace = -4.67f; //-7.0 + (14/6) = -4.67; 14/6 = 2.33            
            static constexpr float OffsetSpace = 2.33f;            
            static constexpr float VelAsterorid = 8.f;            
            static constexpr float TimeAsterorid = 1.5f;
            static constexpr float TimeAsteroridXM = 0.75f;
            static constexpr float TimeAsteroridXH = 0.6f;
            static constexpr float VelEnemy = 8.f;
            static constexpr float TimeEnemy = 1.5f;
            static constexpr float TimeEnemyXM = 0.75f;
            static constexpr float TimeEnemyXH = 0.6f;
            bool m_lastStage = false;
            int8_t m_space[SlotDim] = {ESlot::FREE, ESlot::FREE, ESlot::FREE};
            float m_timer;
            Stage m_stage;
            Scene* m_pScene;

            inline float getPosX(int index)
            {
                return StartOffsetSpace + index * OffsetSpace;
            }

            int weightedRandom(int* weight, int dim)
            {
                float rand =static_cast<float>(PRNG::getNumber()) / 0xFFFF'FFFF; 
                float comulative = 0.f;

                for(int i = 0; i < dim; i++)
                {
                    comulative += weight[i];
                    if(comulative >= rand)
                        return i; 
                }

                return dim-1;
            }

            void getAvailableSlot(int* available, int& dim)
            {
                dim = 0;    
                for(int i = 0; i < SlotDim; i++)
                    if(m_space[i] = ESlot::FREE)
                        available[dim++] = i;
            }

            int pickSlot(int prev, int index, int spawnCount)
            {
                if(spawnCount == SlotDim)
                    return  index;
                
                return (prev + 1 + PRNG::getNumber() % (SlotDim - 1)) % SlotDim; 
            }

            void spawnEntities(Stage& stage, uint32_t spawnCount)
            {
                
                if(stage.eStage >= ESpawnState::SPAWN_ASTEROID_EASY && stage.eStage <= ESpawnState::SPAWN_ASTEROID_HARD)
                {
                    int prev = -1;
                    
                    for(int i = 0; i < spawnCount; i++)
                    {
                        int index = pickSlot(prev, index, spawnCount);
                        m_pScene->requestInstantiate(SpaceScene::m_pAsteroid, Vector3{getPosX(index), 0.f, -100.f});
                        m_space[index] = ESlot::ASTEROID;
                        prev = index;
                    }
                    return;
                }

                int availableSlots[3] = {-1,-1,-1};
                int dim = 0;
                
                getAvailableSlot(availableSlots, dim);
            }

            void spawnLogic()
            {
                //remeber to update timer
                if(m_timer < m_stage.spawnInterval)
                    return;
                m_timer = 0.f;

                uint32_t spawnCount = weightedRandom(m_stage.weights, 3);
                spawnCount = std::min(spawnCount, m_stage.budget);

                spawnEntities(m_stage, spawnCount);

                m_stage.budget -= spawnCount;
            }
    };

    class ScoreSys : public Observer<GameObject, int>
    {
        public:
            void onNotify(const GameObject& entity, const int& event) override;
            void Reset(); 
            uint32_t GetScore() const { return m_score; }
            Text* pTextPoints;
        private:
            uint32_t m_score = 0; 
    };

    class PointSubject : public Subject<GameObject, int>
    {
        public:
            PointSubject();
            void notifyPoints(GameObject& pGameObj, int score);
    };
    

    class SpaceScene : public Scene
    {
            
        public:
            SpaceScene(PhysicsManager* pPhyManager);
            ~SpaceScene();
            void removeHealthIcon();
            void SetPlayer(PlayerShip* player) { m_pPlayer = player; }
            virtual void OnSwitch() override;
            static ScoreSys* pScoreSys;
            static Bullet* pBulletEnemy; 
            void ResetGame();
            void TogglePause();
            void removePauseLayout(UILayout* layout);
            
            uint32_t GetCurrentScore();
            //GESTIONE SPAWN
            static EnemyShip* m_pEnemy;
            static Asteroid* m_pAsteroid;
        private:
            void UpdateScene(float dt) override;
            float randomRange(float min, float max); 
            void handleSpawning(float dt);
            void ResetHealthIcons();

            float m_asteroidTimer = 0.0f;
            float m_enemyTimer = 0.0f;
            // Intervalli di spawn
            float m_asteroidInterval = 3.0f; 
            float m_enemyInterval = 7.0f;
            // Limiti dell'area di gioco dove possono spawnare
            float m_spawnZ = -100.0f; // Lontano dalla camera
            float m_gameAreaX = 10.0f; // Larghezza totale area spawn
            float m_gameAreaY = 10.0f; // Altezza totale area spawn
            float m_elapsedTime = 0.0f;
            unsigned int m_points = 0;
            float m_timer = 0.f;
            bool m_asteroidDebug = false;
            PlayerShip* m_pPlayer = nullptr;
            std::stack<UIBase*> healthIcons;
            UILayout* m_pHUDLayout = nullptr;
           
            PauseScene* m_pPauseScene = nullptr;
            bool m_escProcessed = false;
    };

    class DeathScene : public Scene
    {
        public:
        private:
    };

    class StartupScene : public Scene
    {
        public:
        private:
    };
}