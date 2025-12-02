#pragma once 

#include "gameObject.h"

namespace SpaceEngine
{
    class Player : public GameObject
    { 
        Player(std::string filePathModel);
    };
}
