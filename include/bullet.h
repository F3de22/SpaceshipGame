#pragma once
#include "gameObject.h"

namespace SpaceEngine
{
    class Bullet : public GameObject
    {
        public:
            Bullet(Scene* pScene, std::string filePathModel);
            ~Bullet() = default;
    };
}