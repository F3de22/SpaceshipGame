#include "collisionDetection.h"

namespace SpaceEngine
{
    
    
    void PhysicsManager::AddColliders(const std::list<Collider*>& lCols)
    {
        lColliders.insert(lColliders.end(), lCols.begin(), lCols.end());
        
        for(Collider* col : lCols)
        {
            grid.AddColliderToHGrid(col);
        }
    }
    
    void PhysicsManager::Step(float fixed_dt)
    {
        uint32_t occupiedLevelsMask = grid.occupiedLevelsMask;

        
        for(Collider* col : lColliders)
        {
            if(!col->gameObj->pendingDestroy)
            {
                col->gameObj->fixedUpdate(fixed_dt);
                //verify if the pos change, if yes update(remove and insert) the hgrid
                Vector3 pos = col->gameObj->getComponent<Transform>()->getWorldPosition();
                if(col->pos != pos)
                {
                    col->pos = pos;
                    grid.RemoveObjectFromGrid(col);
                    grid.AddColliderToHGrid(col);
                }   
            }
        }

        for(Collider* col : lColliders)
        {
            if(!col->gameObj->pendingDestroy)
                grid.CheckObjAgainstGrid(col, 0);
        }

        //check collision on hgrid
    }


}
