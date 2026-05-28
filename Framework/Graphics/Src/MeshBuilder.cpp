#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;

namespace
{
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] = {
            Colors::Red,
            Colors::Aqua,
            Colors::Yellow,
            Colors::Green,
            Colors::Purple,
            Colors::Blue,
            Colors::Pink,
            Colors::Orange,
            Colors::Indigo,
            Colors::DarkSalmon
        };

        index = (index + 1) % std::size(colorTable);
        return colorTable[index];
    }

    void CreateCubeIndices(std::vector<uint32_t>& indices)
    {
        indices =
        {
            // front
            0, 1, 2,
            0, 2, 3,
            // back
            7, 5, 4,
            7, 6, 5,
            // right
            3, 2, 6,
            3, 6, 7,
            // left
            4, 5, 1,
            4, 1, 0,
            // top
            1, 5, 6,
            1, 6, 2,
            // bottom
            0, 3, 7,
            0, 7, 4
        };
    }

    void CreatePlaneIndices(std::vector<uint32_t>& indices, int numRows, int numColumns)
    {
        for (int r = 0; r < numRows; ++r)
        {
            for (int c = 0; c < numColumns; ++c)
            {
                int i = (r * (numColumns + 1)) + c;
                // triangle 0
                indices.push_back(i);
                indices.push_back(i + numColumns + 1);
                indices.push_back(i + numColumns + 2);

                // triangles 1
                indices.push_back(i);
                indices.push_back(i + numColumns + 2);
                indices.push_back(i + 1);
            }
        }
    }
}

MeshPC MeshBuilder::CreateVertexCubePC(float size, const Color& color)
{
    MeshPC mesh;
    const float hs = size * 0.5f;

    // Front
    mesh.vertices.push_back({ { -hs,  hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs, -hs, -hs } , { color } });
    mesh.vertices.push_back({ { -hs, -hs, -hs } , { color } });

    mesh.vertices.push_back({ { -hs,  hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs, -hs, -hs } , { color } });

    // Back
    mesh.vertices.push_back({ { -hs, -hs,  hs } , { color } });
    mesh.vertices.push_back({ {  hs, -hs,  hs } , { color } });
    mesh.vertices.push_back({ { -hs,  hs,  hs } , { color } });

    mesh.vertices.push_back({ {  hs, -hs,  hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs,  hs } , { color } });
    mesh.vertices.push_back({ { -hs,  hs,  hs } , { color } });

    // Left
    mesh.vertices.push_back({ { -hs,  hs,  hs } , { color } });
    mesh.vertices.push_back({ { -hs, -hs, -hs } , { color } });
    mesh.vertices.push_back({ { -hs, -hs,  hs } , { color } });

    mesh.vertices.push_back({ { -hs,  hs,  hs } , { color } });
    mesh.vertices.push_back({ { -hs,  hs, -hs } , { color } });
    mesh.vertices.push_back({ { -hs, -hs, -hs } , { color } });

    // Right
    mesh.vertices.push_back({ {  hs, -hs,  hs } , { color } });
    mesh.vertices.push_back({ {  hs, -hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs,  hs } , { color } });

    mesh.vertices.push_back({ {  hs, -hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs,  hs } , { color } });

    // Top
    mesh.vertices.push_back({ { -hs,  hs,  hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs, -hs } , { color } });
    mesh.vertices.push_back({ { -hs,  hs, -hs } , { color } });

    mesh.vertices.push_back({ { -hs,  hs,  hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs,  hs } , { color } });
    mesh.vertices.push_back({ {  hs,  hs, -hs } , { color } });

    // Bottom
    mesh.vertices.push_back({ { -hs, -hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs, -hs, -hs } , { color } });
    mesh.vertices.push_back({ { -hs, -hs,  hs } , { color } });

    mesh.vertices.push_back({ {  hs, -hs, -hs } , { color } });
    mesh.vertices.push_back({ {  hs, -hs,  hs } , { color } });
    mesh.vertices.push_back({ { -hs, -hs,  hs } , { color } });

    return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
    MeshPC mesh;
    const float hs = size * 0.5f;
    int index = rand() % 100;

    // Front
    mesh.vertices.push_back({ { -hs, -hs, -hs } , { GetNextColor(index) } });
    mesh.vertices.push_back({ { -hs,  hs, -hs } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hs,  hs, -hs } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hs, -hs, -hs } , { GetNextColor(index) } });

    // Back
    mesh.vertices.push_back({ { -hs, -hs,  hs } , { GetNextColor(index) } });
    mesh.vertices.push_back({ { -hs,  hs,  hs } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hs,  hs,  hs } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hs, -hs,  hs } , { GetNextColor(index) } });

    // add the indices
    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPX MeshBuilder::CreateCubePX(float size)
{
    MeshPX mesh;

    const float hs = size * 0.5f;
    const float ot = 1.0f / 3.0f;
    const float tt = 2.0f / 3.0f;

    // front
    mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });

    mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });
    mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });

    // right
    mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });
    mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });
    mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.75f, ot} });

    mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });
    mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.75f, ot} });
    mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.75f, tt} });

    // back
    mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.75f, tt} });
    mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.75f, ot} });
    mesh.vertices.push_back({ { -hs,  hs,  hs}, {1.0f, ot} });

    mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.75f, tt} });
    mesh.vertices.push_back({ { -hs,  hs,  hs}, {1.0f, ot} });
    mesh.vertices.push_back({ { -hs, -hs,  hs}, {1.0f, tt} });

    // left
    mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { -hs,  hs,  hs}, {0.0f, ot} });
    mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });

    mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { -hs, -hs,  hs}, {0.0f, tt} });
    mesh.vertices.push_back({ { -hs,  hs,  hs}, {0.0f, ot} });

    // top
    mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ { -hs,  hs,  hs}, {0.25f,0.0f} });
    mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.5f,0.0f} });

    mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.5f,0.0f} });
    mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });

    // bottom
    mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.5f,1.0f} });
    mesh.vertices.push_back({ { -hs, -hs,  hs}, {0.25f,1.0f} });

    mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });
    mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.5f,1.0f} });

    // dont need indices as the vertices make up the shape

    return mesh;
}

MeshPC MeshBuilder::CreateBoxPC(float width, float height, float depth)
{
    MeshPC mesh;
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hd = depth * 0.5f;
    int index = rand() % 100;

    // front
    mesh.vertices.push_back({ { -hw, -hh, -hd } , { GetNextColor(index) } });
    mesh.vertices.push_back({ { -hw,  hh, -hd } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hw,  hh, -hd } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hw, -hh, -hd } , { GetNextColor(index) } });

    // back
    mesh.vertices.push_back({ { -hw, -hh,  hd } , { GetNextColor(index) } });
    mesh.vertices.push_back({ { -hw,  hh,  hd } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hw,  hh,  hd } , { GetNextColor(index) } });
    mesh.vertices.push_back({ {  hw, -hh,  hd } , { GetNextColor(index) } });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
    MeshPC mesh;
    const float hs = size * 0.5f;
    int index = rand() % 100;


    mesh.vertices.push_back({ {  0.0f,   hs,  0.0f } , { GetNextColor(index) } }); // 0

    mesh.vertices.push_back({ {  -hs,   -hs,  -hs } , { GetNextColor(index) } });  // 1
    mesh.vertices.push_back({ {  -hs,   -hs,   hs } , { GetNextColor(index) } });  // 2
    mesh.vertices.push_back({ {   hs,   -hs,   hs } , { GetNextColor(index) } });  // 3
    mesh.vertices.push_back({ {   hs,   -hs,  -hs } , { GetNextColor(index) } });  // 4

    mesh.indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,

        1, 3, 2,
        1, 4, 3
    };

    return mesh;
}

MeshPC MeshBuilder::CreatePlanePC(int numRows, int numColumns, float spacing, bool horizontal)
{
    MeshPC mesh;
    int index = rand() % 100;
    const float hpw = static_cast<float>(numColumns) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float w = -hpw;
    float h = -hph;
    for (int r = 0; r <= numRows; ++r)
    {
        for (int c = 0; c <= numColumns; ++c)
        {
            Math::Vector3 pos = (horizontal) ? Math::Vector3{ w, 0.0f, h } : Math::Vector3{ w, h, 0.0f };
            mesh.vertices.push_back({ pos ,  GetNextColor(index) });
            w += spacing;
        }
        w = -hpw;
        h += spacing;
    }

    CreatePlaneIndices(mesh.indices, numRows, numColumns);

    return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(int numRows, int numColumns, float spacing, bool horizontal)
{
    MeshPC mesh;
    int index = rand() % 100;
    const float hpw = static_cast<float>(numColumns) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numColumns);
    const float vInc = -1.0f / static_cast<float>(numRows);

    float w = -hpw;
    float h = -hph;
    float u = 0.0f;
    float v = 1.0f;

    for (int r = 0; r <= numRows; ++r)
    {
        for (int c = 0; c <= numColumns; ++c)
        {
            Math::Vector3 pos = (horizontal) ? Math::Vector3{ w, 0.0f, h } : Math::Vector3{ w, h, 0.0f };
            mesh.vertices.push_back({ pos , {u, v} });
            w += spacing;
            u += uInc;
        }
        w = -hpw;
        h += spacing;
        v += vInc;
        u = 0.0f;
    }

    CreatePlaneIndices(mesh.indices, numRows, numColumns);

    return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(int slices, int rings)
{
    MeshPC mesh;
    int index = rand() % 100;

    const float hh = static_cast<float>(rings) * 0.5f;
    const float fSlices = static_cast<float>(slices);
    for (int r = 0; r <= rings; ++r)
    {
        float rF = static_cast<float>(r);
        for (int s = 0; s <= slices; ++s)
        {
            float sF = static_cast<float>(s);
            float rotation = (sF / fSlices) * Math::Constants::TwoPi;

            mesh.vertices.push_back({ {sin(rotation), rF - hh, -cos(rotation) }, GetNextColor(index) });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC2(int slices, int rings)
{
    MeshPC mesh;
    int index = rand() % 100;

    const float hh = static_cast<float>(rings) * 0.5f;
    const float fSlices = static_cast<float>(slices);

    for (int r = 0; r <= rings; ++r)
    {
        float rF = static_cast<float>(r);

        for (int s = 0; s <= slices; ++s)
        {
            float sF = static_cast<float>(s);
            float rotation = (sF / fSlices) * Math::Constants::TwoPi;

            mesh.vertices.push_back({ { sin(rotation), rF - hh, -cos(rotation) },GetNextColor(index) });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    uint32_t bottomTriangleCenterIndice = mesh.vertices.size();
    uint32_t bottomStartIndice = 0;
    mesh.vertices.push_back({ { 0.0f, -hh, 0.0f },GetNextColor(index) });

    uint32_t topTriangleCenterIndice = mesh.vertices.size();
    uint32_t topStartIndice = rings * (slices + 1);
    mesh.vertices.push_back({ { 0.0f, hh, 0.0f },GetNextColor(index) });

    for (int s = 0; s < slices; ++s)
    {
        mesh.indices.push_back(bottomTriangleCenterIndice);
        mesh.indices.push_back(bottomStartIndice + s);
        mesh.indices.push_back(bottomStartIndice + s + 1);

        mesh.indices.push_back(topTriangleCenterIndice);
        mesh.indices.push_back(topStartIndice + s + 1); // opposite order to render the correct direction
        mesh.indices.push_back(topStartIndice + s);
    }

    return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(int slices, int rings, int radius)
{
    MeshPC mesh;
    int index = rand() % 100;

    float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
    float horzRotation = Math::Constants::TwoPi / static_cast<float>(slices);

    for (int r = 0; r <= rings; ++r)
    {
        float rF = static_cast<float>(r);
        float phi = rF * vertRotation;
        for (int s = 0; s <= slices; ++s)
        {
            float sF = static_cast<float>(s);
            float rotation = sF * horzRotation;

            mesh.vertices.push_back({ {
                    radius * sin(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * cos(rotation) * sin(phi)}, GetNextColor(index) });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}