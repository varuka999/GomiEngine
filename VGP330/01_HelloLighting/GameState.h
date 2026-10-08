#pragma once

#include <GomiEngine/Inc/GomiEngine.h>

using namespace GomiEngine;
using namespace GomiEngine::Graphics;

class GameState : public AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;
private:
    void UpdateCamera(float deltaTime);

    Camera mCamera;
    DirectionalLight mDirectionalLight;
    StandardEffect mStandardEffect;

    RenderObject mRenderObject;
};
