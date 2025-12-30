#pragma once
#include "gameObject.h"
#include "utils/utils.h"
#include "collisionDetection.h"
#include "renderer.h"
#include "inputManager.h"
#include "bullet.h"

namespace SpaceEngine {


    class PlayerShip : public GameObject {
    public:
        PlayerShip(Scene* pScene, std::string filePathModel);
        virtual ~PlayerShip();

        void Init();
        void MoveUp();
        void MoveDown();
        void MoveLeft();
        void MoveRight();
        void Fire();

        // Gestisce l'input e aggiorna la posizione
        virtual void update(float dt) override;
        virtual void onCollisionEnter(Collider* col) override;
        RenderObject getRenderObject();

    private:
        void HandleInput(float dt);
        
        Bullet* m_pBullet = nullptr;
        float m_speed;
        float m_dt = 0.f;
        // Limiti di movimento
        float m_limitX;
        float m_limitY;

        float m_shootCooldown;
    };

    class MoveUpCommand : public Command
    {
        public:
            virtual void execute(void* actor) override
            {
                PlayerShip* ship = static_cast<PlayerShip*>(actor);
                ship->MoveUp();
            }
    };

    class MoveDownCommand : public Command
    {
        public:
            virtual void execute(void* actor) override
            {
                PlayerShip* ship = static_cast<PlayerShip*>(actor);
                ship->MoveDown();
            }
    };

    class MoveLeftCommand : public Command
    {
        public:
            virtual void execute(void* actor) override
            {
                PlayerShip* ship = static_cast<PlayerShip*>(actor);
                ship->MoveLeft();
            }
    };

    class MoveRightCommand : public Command
    {
        public:
            virtual void execute(void* actor) override
            {
                PlayerShip* ship = static_cast<PlayerShip*>(actor);
                ship->MoveRight();
            }
    };

    class FireCommand : public Command
    {
        public:
            virtual void execute(void* actor) override
            {
                PlayerShip* ship = static_cast<PlayerShip*>(actor);
                ship->Fire();
            }
    };
}