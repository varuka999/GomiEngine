#include "Precompiled.h"
#include "RenderObject.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;

void RenderObject::Terminate()
{
    meshBuffer.Terminate();
}