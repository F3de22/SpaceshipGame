#include "asteroid.h"
#include <iostream>
#include <ctime>

namespace SpaceEngine {

    Asteroid::Asteroid(std::string filePathModel) {
        m_pMesh = MeshManager::loadMesh(filePathModel);
        BaseMaterial* pMat = m_pMesh->getMaterialBySubMeshIndex(0);
        pMat->pShader = ShaderManager::findShaderProgram("simpleTex");
        m_pCollider = new Collider(this);

        m_rotationSpeed = 0.0f;
        m_velocity = 0.0f;
        m_spawnZ = -100.0f;
        m_despawnZ = 20.0f;     // Arriva fino a dietro la camera

        m_spawnRangeX = 50.0f;  // Range orizzontale di spawn
        m_spawnRangeY = 30.0f;  // Range verticale di spawn

        // Assegna un asse di rotazione casuale
        srand(static_cast<unsigned int>(time(0)));
        m_rotationAxis = glm::normalize(glm::vec3(
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
            static_cast <float>(rand()) / static_cast <float> (RAND_MAX)));
    }

    Asteroid::~Asteroid() {
        if(m_pMesh) delete m_pMesh;
    }

    void Asteroid::Init() {
        if (m_pTransform) {
            m_pTransform->setLocalScale(glm::vec3(2.0f)); // Dimensione di base
        }

        Spawn(); // Appena creato, posizionalo subito
    }

    void Asteroid::Spawn() {
        // Genera X e Y casuali
        
        float randomX = -m_spawnRangeX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (m_spawnRangeX * 2)));
        float randomY = -m_spawnRangeY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (m_spawnRangeY * 2)));

        if (m_pTransform) {
            m_pTransform->setLocalPosition(glm::vec3(randomX, randomY, m_spawnZ));
            
            float scaleVar = 1.0f + static_cast<float>(rand()) / (RAND_MAX / 1.0f);
            m_pTransform->setLocalScale(glm::vec3(scaleVar));
        }

        // Velocità leggermente diversa (es. tra 10 e 25)
        m_velocity = 10.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0f));
        
        // Rotazione casuale
        m_rotationSpeed = 30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 60.0f));
        m_rotationAxis = glm::normalize(glm::vec3((float)rand(), (float)rand(), (float)rand()));
    }

    void Asteroid::update(float dt) {
        if (!m_pTransform) return;

        // movimento in avanti (verso il player)
        glm::vec3 currentPos = m_pTransform->getLocalPosition();
        currentPos.z += m_velocity * dt;
        m_pTransform->setLocalPosition(currentPos);

        // 2. Rotazione su se stesso (effetto visivo)
        m_pTransform->rotateLocal(m_rotationSpeed * dt, m_rotationAxis);

        // 3. Controllo Uscita Schermo (Riciclo)
        if (currentPos.z > m_despawnZ) {
            Spawn(); // Resetta e ricomincia dal fondo
        }
    }

    RenderObject Asteroid::getRenderObject() {
        RenderObject renderObj;
        renderObj.mesh = m_pMesh;
        glm::mat4 model = glm::mat4(1.0f);

        if (m_pTransform) {
            model = glm::translate(model, m_pTransform->getLocalPosition());

            glm::mat4 rot_mat = glm::mat4_cast(m_pTransform->getLocalRotation());
            
            model = model * rot_mat;
            
            model = glm::scale(model, m_pTransform->getLocalScale());
        }

        renderObj.modelMatrix = model;
        return renderObj;
    }

    void Asteroid::onCollisionEnter(Collider* col) {
        SPACE_ENGINE_INFO("PlayerShip Collision onEnter Called with Collider: {}", reinterpret_cast<std::uintptr_t>(col));
    }
}