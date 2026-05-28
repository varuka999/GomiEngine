#include "ShapeStates.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Input;

void ShapeStates::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // create shape
    CreateShapes();
    mMeshBuffer.Initialize(mMesh);

    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTransform.fx";
    mVertexShader.Initialize<VertexPC>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mConstantBuffer.Initialize(sizeof(Math::Matrix4));
}
void ShapeStates::Move(float deltaTime)
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
void ShapeStates::Terminate()
{
    mConstantBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer.Terminate();
}
void ShapeStates::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("SphereState");
    }
}
void ShapeStates::Render()
{
    // prepare the GPU
    mVertexShader.Bind();
    mPixelShader.Bind();

    // sync buffer information
    mConstantBuffer.BindVS(0);

    // update max buffers
    Math::Matrix4 matWorld = Math::Matrix4::Identity;
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    Math::Matrix4 wvp = matWorld * matView * matProj;
    wvp = Math::Transpose(wvp);
    mConstantBuffer.Update(&wvp);

    // render object
    mMeshBuffer.Render();
}
void ShapeStates::CreateShapes()
{
    mMesh = MeshBuilder::CreateVertexCubePC(1.0f, Colors::Green);
    //mMesh = MeshBuilder::CreateCubePC(1.0f);
    //mMesh = MeshBuilder::CreateBoxPC(1.5f, 1.0f, 2.0f);
    //mMesh = MeshBuilder::CreatePyramidPC(1.0f);
    //mMesh = MeshBuilder::CreatePlanePC(5.0f, 10, 1, false);
    //mMesh = MeshBuilder::CreateCylinderPC(16, 2);
    //mMesh = MeshBuilder::CreateSpherePC(30, 30, 1);
}

void VertexCubeState::CreateShapes()
{
    mMesh = MeshBuilder::CreateVertexCubePC(1.0f, Colors::Green);
}
void VertexCubeState::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("CubeState");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("SphereState");
    }
}
void CubeState::CreateShapes()
{
    mMesh = MeshBuilder::CreateCubePC(1.0f);
}
void CubeState::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("BoxState");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("VertexCubeState");
    }
}
void BoxState::CreateShapes()
{
    mMesh = MeshBuilder::CreateBoxPC(1.5f, 1.0f, 2.0f);
}
void BoxState::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("PyramidState");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("CubeState");
    }
}
void PyramidState::CreateShapes()
{
    mMesh = MeshBuilder::CreatePyramidPC(1.0f);
}
void PyramidState::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("PlaneState");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("BoxState");
    }
}
void PlaneState::CreateShapes()
{
    mMesh = MeshBuilder::CreatePlanePC(5.0f, 5, 1, false);
}
void PlaneState::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("CylinderState");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("PyramidState");
    }
}
void CylinderState::CreateShapes()
{
    mMesh = MeshBuilder::CreateCylinderPC2(8, 3);
}
void CylinderState::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("SphereState");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("PlaneState");
    }
}
void SphereState::CreateShapes()
{
    mMesh = MeshBuilder::CreateSpherePC(16, 16, 1);
}
void SphereState::Update(float deltaTime)
{
    Move(deltaTime);

    if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    {
        MainApp().ChangeState("VertexCubeState");
    }
    if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    {
        MainApp().ChangeState("CylinderState");
    }
}