#include "GameState.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Input;

void GameState::Initialize()
{
    // set up camera
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // set up lighting
    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    
    // set up render object
    Mesh mesh = MeshBuilder::CreateSphere(30, 30, 1.0f);
    mRenderObject.meshBuffer.Initialize(mesh);

    // set up the effect
    std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderPath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}
void GameState::Terminate()
{
    mStandardEffect.Terminate();
    mRenderObject.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}
void GameState::Render()
{
    SimpleDraw::AddGroundPlane(20.0f, Colors::LightGray);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        mStandardEffect.Render(mRenderObject);
    mStandardEffect.End();
}
void GameState::DebugUI()
{
    ImGui::Begin("Hello Lighting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mStandardEffect.DebugUI();
    ImGui::End();
}
void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 1.5f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}
