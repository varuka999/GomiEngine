#pragma once


#include "MeshTypes.h"

namespace GomiEngine::Graphics
{
    class MeshBuilder
    {
    public:
        // cube
        static MeshPC CreateVertexCubePC(float size, const Color& color);
        static MeshPC CreateCubePC(float size);

        // rectangle/box
        static MeshPC CreateBoxPC(float width, float height, float depth);

        // pyramid
        static MeshPC CreatePyramidPC(float size);

        // plane
        static MeshPC CreatePlanePC(int numRows, int numColumns, float spacing, bool horizontal = true);

        // cylinder
        static MeshPC CreateCylinderPC(int slices, int rings);

        // sphere
        static MeshPC CreateSpherePC(int slices, int rings, int radius);
    };
}
