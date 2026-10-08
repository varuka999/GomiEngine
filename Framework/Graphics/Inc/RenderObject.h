#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"


namespace GomiEngine::Graphics
{
    class RenderObject
    {
    public:
        void Terminate();

        MeshBuffer meshBuffer; // shape, vertices, indices...
        Transform transform;   // location/orientation
        Material material;     // light data
    };
}