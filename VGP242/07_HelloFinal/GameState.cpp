#include "GameState.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -400.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Make the Planets (NEW VERSION)
    mSunPlanet.Initialize(50, 1, 1, 0, "planets/sun.jpg");
    mEarthPlanet.Initialize(12, 1, 1, 100, "planets/earth/earth.jpg");
    mEarthPlanet2.Initialize(6, 1, 1, 45, "planets/earth/earth.jpg", &mEarthPlanet);


    // Make the Planets (OLD VERSION)
    mSpacePX = MeshBuilder::CreateSkySpherePX(128, 128, 1000);
    mSunPX = MeshBuilder::CreateSpherePX(128, 128, 50);
    mMercuryPX = MeshBuilder::CreateSpherePX(64, 64, 5);
    mVenusPX = MeshBuilder::CreateSpherePX(64, 64, 12);
    mEarthPX = MeshBuilder::CreateSpherePX(64, 64, 13);
    mMarsPX = MeshBuilder::CreateSpherePX(64, 64, 7);

    mSpaceMeshBuff.Initialize(mSpacePX);
    mSunMeshBuff.Initialize(mSunPX);
    mMercuryMeshBuff.Initialize(mMercuryPX);
    mVenusMeshBuff.Initialize(mVenusPX);
    mEarthMeshBuff.Initialize(mEarthPX);
    mMarsMeshBuff.Initialize(mMarsPX);

    mMercuryPosition = { 90.0f, 0.0f, 0.0f };
    mVenusPosition = { 120.0f, 0.0f, 0.0f };
    mEarthPosition = { 160.0f, 0.0f, 0.0f };
    mMarsPosition = { 200.0f, 0.0f, 0.0f };

    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mConstantBuffer.Initialize(sizeof(Math::Matrix4));
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    mSpaceSkyboxTexture = TextureManager::Get()->LoadTexture("skysphere/space.jpg");
    mSunTexture = TextureManager::Get()->LoadTexture("planets/sun.jpg");
    mMercuryTexture = TextureManager::Get()->LoadTexture("planets/mercury.jpg");
    mVenusTexture = TextureManager::Get()->LoadTexture("planets/venus.jpg");
    mEarthTexture = TextureManager::Get()->LoadTexture("planets/earth/earth.jpg");
    mMarsTexture = TextureManager::Get()->LoadTexture("planets/mars.jpg");
}
void GameState::Terminate()
{
    mEarthPlanet2.Terminate();
    mEarthPlanet.Terminate();
    mSunPlanet.Terminate();

    TextureManager::Get()->ReleaseTexture(mSpaceSkyboxTexture);
    TextureManager::Get()->ReleaseTexture(mSunTexture);
    TextureManager::Get()->ReleaseTexture(mMercuryTexture);
    TextureManager::Get()->ReleaseTexture(mVenusTexture);
    TextureManager::Get()->ReleaseTexture(mEarthTexture);
    TextureManager::Get()->ReleaseTexture(mMarsTexture);
    mSampler.Terminate();
    mConstantBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}
void GameState::Update(float deltaTime)
{
    mEarthPlanet.Update(deltaTime);

    UpdateCamera(deltaTime);
}

enum class Shape
{
    None,
    AABB,
    AABBFilled,
    Sphere,
    GroundPlane,
    GroundCircle,
    Transform,
};
const char* gShapeNames[] =
{
    "None",
    "AABB",
    "AABBFilled",
    "Sphere",
    "GroundPlane",
    "GroundCircle",
    "Transform"
};

void GameState::Render()
{
    // prepare the GPU
    mVertexShader.Bind();
    mPixelShader.Bind();

    // sync buffer information
    mConstantBuffer.BindVS(0);
    mSampler.BindPS(0);

    PlanetRenders();

    // render object
    //mMeshBuffer1.Render();

    //SimpleDraw::Render(mCamera);
}
void GameState::PlanetRenders()
{
    {
        // Space
        Math::Matrix4 matWorld = Math::Matrix4::Identity;
        Math::Matrix4 matView = mCamera.GetViewMatrix();
        Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
        Math::Matrix4 wvp = matWorld * matView * matProj;
        wvp = Math::Transpose(wvp);
        TextureManager::Get()->BindPS(mSpaceSkyboxTexture, 0);

        mConstantBuffer.Update(&wvp);
        mSpaceMeshBuff.Render();
    }

    mSunPlanet.Render(mCamera, mConstantBuffer);
    mEarthPlanet.Render(mCamera, mConstantBuffer);
    mEarthPlanet2.Render(mCamera, mConstantBuffer);

    //{
    //    // Sun
    //    Math::Matrix4 matWorld = Math::Matrix4::Identity;
    //    Math::Matrix4 matView = mCamera.GetViewMatrix();
    //    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    //    Math::Matrix4 wvp = matWorld * matView * matProj;
    //    wvp = Math::Transpose(wvp);
    //    mConstantBuffer.Update(&wvp);
    //    TextureManager::Get()->BindPS(mSunTexture, 0);

    //    mSunMeshBuff.Render();
    //}

    //{
    //    // Mercury
    //    Math::Matrix4 matWorld = Math::Matrix4::Translation(mMercuryPosition);
    //    Math::Matrix4 matView = mCamera.GetViewMatrix();
    //    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    //    Math::Matrix4 wvp = matWorld * matView * matProj;
    //    wvp = Math::Transpose(wvp);
    //    mConstantBuffer.Update(&wvp);
    //    TextureManager::Get()->BindPS(mMercuryTexture, 0);

    //    mMercuryMeshBuff.Render();
    //}

    //{
    //    // Venus
    //    Math::Matrix4 matWorld = Math::Matrix4::Translation(mVenusPosition);
    //    Math::Matrix4 matView = mCamera.GetViewMatrix();
    //    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    //    Math::Matrix4 wvp = matWorld * matView * matProj;
    //    wvp = Math::Transpose(wvp);
    //    mConstantBuffer.Update(&wvp);
    //    TextureManager::Get()->BindPS(mVenusTexture, 0);

    //    mVenusMeshBuff.Render();
    //}

    //{
    //    // Earth
    //    Math::Matrix4 matWorld = Math::Matrix4::Translation(mEarthPosition);
    //    Math::Matrix4 matView = mCamera.GetViewMatrix();
    //    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    //    Math::Matrix4 wvp = matWorld * matView * matProj;
    //    wvp = Math::Transpose(wvp);
    //    mConstantBuffer.Update(&wvp);
    //    TextureManager::Get()->BindPS(mEarthTexture, 0);

    //    mEarthMeshBuff.Render();
    //}

    //{
    //    // Mars
    //    Math::Matrix4 matWorld = Math::Matrix4::Translation(mMarsPosition);
    //    Math::Matrix4 matView = mCamera.GetViewMatrix();
    //    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    //    Math::Matrix4 wvp = matWorld * matView * matProj;
    //    wvp = Math::Transpose(wvp);
    //    mConstantBuffer.Update(&wvp);
    //    TextureManager::Get()->BindPS(mMarsTexture, 0);

    //    mMarsMeshBuff.Render();
    //}
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    // CONTENT GOES BETWEEN BEGIN AND END

    ImGui::End();
}
void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
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
void Planet::Initialize(uint32_t planetSize, float rotationSpeed, float orbitRotation, float orbitDistance, std::string texture, Planet* parent)
{
    mMesh = MeshBuilder::CreateSpherePX(64, 64, planetSize);
    mMeshBuffer.Initialize(mMesh);
    mTexture = TextureManager::Get()->LoadTexture(texture);
    mRotationSpeed = rotationSpeed;
    mOrbitRotationSpeed = orbitRotation;
    mOrbitDistance = orbitDistance;

    mParentPlanet = parent;
}
void Planet::Terminate()
{
    TextureManager::Get()->ReleaseTexture(mTexture);
    if (mParentPlanet != nullptr)
    {
        delete mParentPlanet;
        mParentPlanet = nullptr;
    }
}
void Planet::Update(float deltaTime)
{
    mRotation += mRotationSpeed * deltaTime;
    mOrbitRotation += mOrbitRotationSpeed * deltaTime;
}
void Planet::Render(Camera &mCamera, ConstantBuffer &mConstantBuffer)
{
    mMatWorld = GetMatrixWorld();
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    Math::Matrix4 wvp = mMatWorld * matView * matProj;
    wvp = Math::Transpose(wvp);
    mConstantBuffer.Update(&wvp);
    TextureManager::Get()->BindPS(mTexture, 0);

    mMeshBuffer.Render();
}
Matrix4 Planet::GetMatrixWorld()
{
    // local orbit rotation (spins the translation vector around the parent)
    Math::Matrix4 matOrbitRot = Math::Matrix4::RotationY(mOrbitRotation);
    // translate out along the X axis by orbit distance
    Math::Matrix4 matTrans = Math::Matrix4::Translation(mOrbitDistance, 0.0f, 0.0f);
    // local self-rotation (planet rotating on its own axis)
    Math::Matrix4 matSelfRot = Math::Matrix4::RotationY(mRotation);

    // local transform: orbit rotation -> translate -> self rotation
    Math::Matrix4 local = matOrbitRot * matTrans * matSelfRot;

    if (mParentPlanet != nullptr)
    {
        // compose with parent's world matrix so this planet orbits relative to the parent
        return mParentPlanet->GetMatrixWorld() * local;
    }

    return local;
}