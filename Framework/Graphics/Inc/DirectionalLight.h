#pragma once

#include "Color.h"

namespace GomiEngine::Graphics
{
    struct DirectionalLight
    {
        Color ambient = Colors::White;      // when no light is present
        Color diffuse = Colors::White;      // main light color
        Color specular = Colors::White;     // when highlights are clored
        Math::Vector3 direction = Math::Vector3::ZAxis; // direction of the light
        float padding = 0.0f;    // to keep 16 byte aligned
    };
}