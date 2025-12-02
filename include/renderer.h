#pragma once

#include "mesh.h"
#include "utils/utils.h"
#include "material.h"
#include "camera.h"

#include <vector>

namespace SpaceEngine
{

    struct RenderObject
    {
        std::vector<Matrix4>modelMatrix;
        int instances = 0;
        Mesh* mesh = nullptr;
    };

    struct UIRenderObject
    {
        Mesh* mesh = nullptr;
        UIMaterial* material = nullptr;
        Matrix4 modelMatrix = Math::identityMatrix4();
    };

    class Renderer
    {
        public:
            void render(const std::vector<RenderObject>& renderables, BaseCamera& cam);
        private:
            bool debug;
    };

    class UIRenderer
    {
        void render(const std::vector<UIRenderObject>& uiRenderables);
    };
}
