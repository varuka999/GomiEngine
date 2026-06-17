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
    mMeshBuffer1.Initialize(mMesh1);
    mMeshBuffer2.Initialize(mMesh2);
    mMeshBuffer3.Initialize(mMesh3);
    mMeshBuffer4.Initialize(mMesh4);

    mPosition1 = { 10.0f, 10.0f, 10.0f };

    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mConstantBuffer.Initialize(sizeof(Math::Matrix4));

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    mTextureId = TextureManager::Get()->LoadTexture("skysphere/space.jpg");
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
    TextureManager::Get()->ReleaseTexture(mTextureId);
    mSampler.Terminate();
    mConstantBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer1.Terminate();
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
    mSampler.BindPS(0);

    // update max buffers
    Math::Matrix4 matWorld = Math::Matrix4::Identity;
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    Math::Matrix4 wvp = matWorld * matView * matProj;
    wvp = Math::Transpose(wvp);
    mConstantBuffer.Update(&wvp);

    TextureManager::Get()->BindPS(mTextureId, 0);

    // render object
    mMeshBuffer1.Render();

    Math::Matrix4 matWorld2 = Math::Matrix4::Translation(mPosition1);
    Math::Matrix4 matView2 = mCamera.GetViewMatrix();
    Math::Matrix4 matProj2 = mCamera.GetProjectionMatrix();
    Math::Matrix4 wvp2 = matWorld2 * matView2 * matProj2;
    wvp2 = Math::Transpose(wvp2);
    mConstantBuffer.Update(&wvp2);
    TextureManager::Get()->BindPS(mTextureId, 0);
    mMeshBuffer2.Render();

    Math::Matrix4 matWorld3 = Math::Matrix4::Translation({-10.0f, -5.0f, 0.0f});
    Math::Matrix4 matView3 = mCamera.GetViewMatrix();
    Math::Matrix4 matProj3 = mCamera.GetProjectionMatrix();
    Math::Matrix4 wvp3 = matWorld3 * matView3 * matProj3;
    wvp3 = Math::Transpose(wvp3);
    mConstantBuffer.Update(&wvp3);
    TextureManager::Get()->BindPS(mTextureId, 0);
    mMeshBuffer3.Render();


    //// first instance at mPosition
    //Math::Matrix4 matWorld = Math::Matrix4::Translation(mPosition1);
    //Math::Matrix4 wvp = Math::Transpose(matWorld * matView * matProj);
    //mConstantBuffer.Update(&wvp);
    //mMeshBuffer1.Render();

    //// second instance at an offset (example)
    //Math::Matrix4 matWorld2 = Math::Matrix4::Translation(mPosition2 + Math::Vector3{ 2.0f, 0.0f, 0.0f });
    //Math::Matrix4 wvp2 = Math::Transpose(matWorld2 * matView * matProj);
    //mConstantBuffer.Update(&wvp2);
    //mMeshBuffer2.Render();
}
void ShapeStates::CreateShapes()
{
    //mMesh1 = MeshBuilder::CreateVertexCubePC(1.0f, Colors::Green);
    //mMesh1 = MeshBuilder::CreateCubePC(1.0f);
    //mMesh1 = MeshBuilder::CreateBoxPC(1.5f, 1.0f, 2.0f);
    //mMesh1 = MeshBuilder::CreatePyramidPC(1.0f);
    //mMesh1 = MeshBuilder::CreatePlanePC(5.0f, 10, 1, false);
    //mMesh1 = MeshBuilder::CreateCylinderPC(16, 2);
    //mMesh1 = MeshBuilder::CreateSpherePC(30, 30, 1);
}

void VertexCubeState::CreateShapes()
{
    //mMesh1 = MeshBuilder::CreateVertexCubePC(1.0f, Colors::Green);
}
void VertexCubeState::Update(float deltaTime)
{
    Move(deltaTime);

    //if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    //{
    //    MainApp().ChangeState("CubeState");
    //}
    //if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    //{
    //    MainApp().ChangeState("SphereState");
    //}
}
void CubeState::CreateShapes()
{
    //mMesh1 = MeshBuilder::CreateCubePC(1.0f);
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
void CubePXState::CreateShapes()
{
    //mMesh1 = MeshBuilder::CreateCubePX(1.0f);
    mMesh1 = MeshBuilder::CreatePlanePX(10, 10, 1.0f);
}
void CubePXState::Update(float deltaTime)
{
    Move(deltaTime);

    //if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    //{
    //    MainApp().ChangeState("BoxState");
    //}
    //if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    //{
    //    MainApp().ChangeState("VertexCubeState");
    //}
}
void BoxState::CreateShapes()
{
    //mMesh1 = MeshBuilder::CreateBoxPC(1.5f, 1.0f, 2.0f);
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
    //mMesh1 = MeshBuilder::CreatePyramidPC(1.0f);
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
    //mMesh1 = MeshBuilder::CreatePlanePC(5.0f, 5, 1, false);
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
    //mMesh1 = MeshBuilder::CreateCylinderPC2(32, 1);
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
    //mMesh1 = MeshBuilder::CreateSpherePC(16, 16, 1);
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
void SphereXState::CreateShapes()
{
    mMesh1 = MeshBuilder::CreateSpherePX(30, 30, 1);
}
void SphereXState::Update(float deltaTime)
{
    Move(deltaTime);

    //if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    //{
    //    MainApp().ChangeState("VertexCubeState");
    //}
    //if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    //{
    //    MainApp().ChangeState("CylinderState");
    //}
}
void SkySphereXState::CreateShapes()
{
    mMesh1 = MeshBuilder::CreateSkySpherePX(64, 64, 100.0f);
    mMesh2 = MeshBuilder::CreateSpherePX(64, 64, 10.0f);
    mMesh3 = MeshBuilder::CreateSpherePX(64, 64, 2.0f);
    mMesh4 = MeshBuilder::CreateSpherePX(64, 64, 5.0f);
}
void SkySphereXState::Update(float deltaTime)
{
    Move(deltaTime);

    //if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
    //{
    //    MainApp().ChangeState("VertexCubeState");
    //}
    //if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
    //{
    //    MainApp().ChangeState("CylinderState");
    //}
}