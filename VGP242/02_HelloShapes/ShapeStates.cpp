#include "ShapeStates.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Input;

void ShapeStates::Initialize()
{
    CreateShapes();

    mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), mVertices.size());

    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoColor.fx";
    mVertexShader.Initialize<VertexPC>(shaderPath);
    mPixelShader.Initialize(shaderPath);
}
void ShapeStates::Terminate()
{
    mVertices.clear();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer.Terminate();
}
void ShapeStates::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("QuadState");
    }
}
void ShapeStates::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mMeshBuffer.Render();
}
void ShapeStates::CreateShapes()
{
    mVertices.push_back({ {-0.5, -0.5, 0.0,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ {0.0,  0.5, 0.0,}, {GomiEngine::Graphics::Colors::Green} });
    mVertices.push_back({ {0.5, -0.5, 0.0,}, {GomiEngine::Graphics::Colors::Blue} });
}
void QuadStates::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("ShapeState");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("PentaState");
    }
}
void QuadStates::CreateShapes()
{
    mVertices.push_back({ {-0.5f, -0.5f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ {-0.5f,  0.5f, 0.0f,}, {GomiEngine::Graphics::Colors::Green} });
    mVertices.push_back({ { 0.5f,  0.5f, 0.0f,}, {GomiEngine::Graphics::Colors::Blue} });

    mVertices.push_back({ {-0.5f, -0.5f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.5f,  0.5f, 0.0f,}, {GomiEngine::Graphics::Colors::Blue} });
    mVertices.push_back({ { 0.5f, -0.5f, 0.0f,}, {GomiEngine::Graphics::Colors::Green} });
}
void PentaStates::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("PentaState3");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("PentaState2");
    }
}
void PentaStates::CreateShapes()
{
    mVertices.push_back({ {-0.5f,  0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.0f,  0.60f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.5f,  0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });

    mVertices.push_back({ {-0.5f,  0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.5f,  0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.3f, -0.50f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });

    mVertices.push_back({ {-0.5f,  0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.3f, -0.50f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ {-0.3f, -0.50f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
}
void PentaStates2::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("PentaState");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("PentaState3");
    }
}
void PentaStates2::CreateShapes()
{
    mVertices.push_back({ {-0.65f, -0.35f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ {-0.35f,  0.45f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.25f,  0.55f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });

    mVertices.push_back({ {-0.65f, -0.35f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.25f,  0.55f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.65f, -0.10f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });

    mVertices.push_back({ {-0.65f, -0.35f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.65f, -0.10f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.00f, -0.60f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
}
void PentaStates3::Update(float deltaTime)
{
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("PentaState2");
    }
    else if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("PentaState");
    }
}
void PentaStates3::CreateShapes()
{
    mVertices.push_back({ {-0.60f,  0.45f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.60f,  0.45f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.00f, -0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });

    mVertices.push_back({ { 0.60f,  0.45f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.35f, -0.55f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.00f, -0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });

    mVertices.push_back({ {-0.60f,  0.45f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ { 0.00f, -0.15f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
    mVertices.push_back({ {-0.35f, -0.55f, 0.0f,}, {GomiEngine::Graphics::Colors::Red} });
}