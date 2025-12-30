#include "bullet.h"
#include "collisionDetection.h"

namespace SpaceEngine
{
    Bullet::Bullet(Scene* pScene, std::string filePathModel):GameObject(pScene)
    {
        m_pMesh = MeshManager::loadMesh(filePathModel);
        BaseMaterial* pMat = m_pMesh->getMaterialBySubMeshIndex(0);
        pMat->pShader = ShaderManager::findShaderProgram("simpleTex");
        m_pTransform = new Transform();
        m_pCollider = new Collider(this);
    }
}