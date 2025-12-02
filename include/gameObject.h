#pragma once
#include "mesh.h"
#include "transform.h"
#include "material.h"

#include <vector>
#include <string> 

namespace SpaceEngine
{
    class Scene;
    class GameObject 
    {
        public:
            GameObject(){m_numInstances = 1;};
            virtual ~GameObject() = default;
            template<typename T>
            T* getComponent();
            template<typename T>
            void addComponent(T component);
            int getNumInstances();
            void destroy(); 
            virtual void update(float dt) = 0;

        private:
            //Attention
            Scene* scene = nullptr;
        protected:
            GameObject(const std::string& filePathModel);
            int m_numInstances = 0;
            std::vector<Transform*> m_vecTransform;
            Mesh* m_pMesh = nullptr;
            //TODO: add componet for the physics 
    };
}
