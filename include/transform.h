#pragma once

#include "utils/utils.h"

namespace SpaceEngine
{
    class Transform
    {
        public:
            Transform() = default;
            ~Transform() = default;

            Matrix4 getWorldMatrix()
            {
                if(dirty)
                {
                    Matrix4 T = glm::translate(Math::identityMatrix4(), localPos);
                    Matrix4 R = glm::mat4_cast(localRot);
                    Matrix4 S = glm::scale(Math::identityMatrix4(), localScale);
                    Matrix4 localMatrix = T * R * S;
                    
                    if(parent)
                        cachedWorldMatrix = parent->getWorldMatrix() * localMatrix;
                    else 
                        cachedWorldMatrix = localMatrix;
                    
                    dirty = false;
                }
                return cachedWorldMatrix;
            }

            inline void markDirty()
            {
                dirty = true;
            }

            //Local translations
            inline void translateLocal(const Vector3& delta)
            {
                localPos += localRot * delta;
                markDirty();
            }

            inline void translateGlobal(const Vector3& delta)
            {
                if(parent)
                {
                    Matrix4 parentInv = Math::inverse(parent->getWorldMatrix());
                    localPos += Vector3(parentInv * Vector4(delta, 0.f));
                }
                else
                {
                    localPos +=delta;
                }
                markDirty();
            }

            inline void rotateLocal(const float& degree, const Vector3& axis)
            {
                Quat delta = glm::angleAxis(Math::radians(degree), axis);

                localRot = localRot * delta;
                markDirty();
            }

            inline void rotateGlobal(const float& degree, const Vector3& axis)
            {
                Quat delta = glm::angleAxis(Math::radians(degree), axis);
                localRot = delta * localRot;
                markDirty();
            }

            inline void scale(Vector3 t)
            {
                localScale *= t;
                markDirty();
            }

            inline void scale(float x, float y, float z)
            {
                Vector3 v{x, y, z};
                scale(v);
            }

            Vector3 forward() const
            {
                return localRot * Vector3(0, 0, -1);
            }

            Vector3 right() const
            {
                return localRot * Vector3(1, 0, 0);
            }

            Vector3 up() const
            {
                return localRot * Vector3(0, 1, 0);
            }

            Vector3 getWorldPosition()
            {
                Matrix4 world = getWorldMatrix();
                return Vector3(world[3]);
            }

            void setWorldPosition(const Vector3& worldPos)
            {
                if(parent)
                {
                    Matrix4 parentInv = Math::inverse(parent->getWorldMatrix());
                    localPos = Vector3(parentInv * Vector4(worldPos, 1.f));
                }
                else
                {
                    localPos = worldPos;
                }

                markDirty();
            }

            Vector3 getWorldRotationEuler()
            {
                Matrix4 world = getWorldMatrix();
                Quat q = glm::quat_cast(world);

                return glm::eulerAngles(q);
            }

            Vector3 getWorldRotationEulerDegree()
            {
                return glm::degrees(getWorldRotationEuler());
            }

        public:
            Vector3 localPos{0.f};
            Quat localRot{1.f, 0.f, 0.f, 0.f};
            Vector3 localScale{1.f};
            Transform* parent = nullptr;
        private:
            bool dirty = true;
            Matrix4 cachedWorldMatrix = Math::identityMatrix4();
    };
}