#include "GameState.h"
#include <algorithm>

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Input;

int gPlanetSizeScaler = 1.0f;

// ImGUI
float globalSpeed = 1.0f;
bool orbitRings = true;
int selectedPlanet = 3;
bool lockToPlanet = false;
float gMoveSpeed = 5000.0f;
float cameraLockDistanceY = 0.0f;
float cameraLockDistanceZ = 100.0f;
float cameraLockDistanceZMin = 0.0f;
float cameraLockDistanceZMax = 3000.0f;
float cameraFollowYaw = 0.0f;
float cameraFollowPitch = 0.25f;

bool planetMarkers = false;
float markerScale = 10.0f;
float markerLineScale = 30.0f;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 10000.0f, -60000.0f });
    mCamera.SetLookAt({ 0.0f, -135.0f, 0.0f });

    mCamera.SetNearPlane(1.0f);
    mCamera.SetFarPlane(1e7f);

    mSpacePX = MeshBuilder::CreateSkySpherePX(1280, 1280, 700000);
    mSpaceMeshBuff.Initialize(mSpacePX);

    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mConstantBuffer.Initialize(sizeof(Math::Matrix4));
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    mSpaceSkyboxTexture = TextureManager::Get()->LoadTexture("skysphere/space.jpg");

    mPlanets.reserve(64);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Sun", 1391, 0.040f, 0.0f, 0.0f, "planets/sun.jpg");

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Mercury", 5, 0.017f, 0.01137f, 5790.0f, "planets/mercury.jpg", 0);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Venus", 12, -0.004f, 0.00445f, 10800.0f, "planets/venus.jpg", 0);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Earth", 13, 1.000f, 0.00274f, 14900.0f, "planets/earth/earth.jpg", 0);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Moon", 3, 0.037f, 0.03660f, 384.4f, "planets/pluto.jpg", 3);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Mars", 7, 0.972f, 0.00146f, 22790.0f, "planets/mars.jpg", 0);
        
    mPlanets.emplace_back();
    mPlanets.back().Initialize("Jupiter", 143, 2.412f, 0.000231f, 77860.0f, "planets/jupiter.jpg", 0);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Saturn", 130, 2.246f, 0.000093f, 143350.0f, "planets/saturn.jpg", 0);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Uranus", 51, -1.388f, 0.0000326f, 287250.0f, "planets/uranus.jpg", 0);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Neptune", 49, 1.486f, 0.0000166f, 449510.0f, "planets/neptune.jpg", 0);

    mPlanets.emplace_back();
    mPlanets.back().Initialize("Pluto", 2, -0.156f, 0.0000110f, 587000.0f, "planets/pluto.jpg", 0);
}
void GameState::Terminate()
{
    TextureManager::Get()->ReleaseTexture(mSpaceSkyboxTexture);
    for (auto& planet : mPlanets)
    {
        planet.Terminate();
    }

    mSampler.Terminate();
    mConstantBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}
void GameState::Update(float deltaTime)
{
    for (size_t i = 0; i < mPlanets.size(); ++i)
    {
        Vector3 parentPosition = Vector3::Zero;

        int parentIndex = mPlanets[i].GetParentIndex();
        if (parentIndex >= 0)
        {
            parentPosition = mPlanets[parentIndex].GetPosition();
        }

        mPlanets[i].SetParentPosition(parentPosition);
        mPlanets[i].Update(deltaTime);
    }

    if (lockToPlanet)
    {
        UpdateFollowCamera(deltaTime);
    }
    else
    {
        UpdateCamera(deltaTime);
    }
}

void GameState::Render()
{
    // prepare the GPU
    mVertexShader.Bind();
    mPixelShader.Bind();

    // sync buffer information
    mConstantBuffer.BindVS(0);
    mSampler.BindPS(0);

    PlanetRenders();

    SimpleDraw::Render(mCamera);
}
void GameState::PlanetRenders()
{
    // Space
    Math::Matrix4 matWorld = Math::Matrix4::Translation(mCamera.GetPosition());
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();

    Math::Matrix4 wvp = matWorld * matView * matProj;
    wvp = Math::Transpose(wvp);

    TextureManager::Get()->BindPS(mSpaceSkyboxTexture, 0);
    mConstantBuffer.Update(&wvp);
    mSpaceMeshBuff.Render();

    for (auto& planet : mPlanets)
    {
        planet.Render(mCamera, mConstantBuffer);
    }

    if (orbitRings)
    {
        DrawOrbitRings();
    }

    if (planetMarkers)
    {
        DrawPlanetMarkers();
    }
}
void GameState::DrawOrbitRings()
{
    for (const auto& planet : mPlanets)
    {
        if (planet.GetOrbitDistance() <= 0.0f)
        {
            continue;
        }

        Vector3 center = Vector3::Zero;

        int parentIndex = planet.GetParentIndex();
        if (parentIndex >= 0)
        {
            center = mPlanets[parentIndex].GetPosition();
        }

        SimpleDraw::AddGroundCircle(2560, planet.GetOrbitDistance(), center, Colors::White);
    }
}
void GameState::DrawPlanetMarkers()
{
    for (size_t i = 0; i < mPlanets.size(); ++i)
    {
        if (i == 0)
        {
            continue;
        }

        const Planet& planet = mPlanets[i];

        Vector3 position = planet.GetPosition();
        float markerRadius = planet.GetPlanetSize() * markerScale;
        float markerLineHeight = planet.GetPlanetSize() * markerLineScale;

        SimpleDraw::AddSphere(24, 12, markerRadius, position, Colors::White);
        SimpleDraw::AddLine(position + Vector3{ 0.0f, -markerLineHeight, 0.0f }, position + Vector3{ 0.0f,  markerLineHeight, 0.0f }, Colors::Yellow);
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Solar System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SliderFloat("Move Speed", &gMoveSpeed, 1000.0f, 10000.0f);
    ImGui::SliderFloat("Global Speed", &globalSpeed, 0.0f, 100.0f);
    ImGui::Checkbox("Show Orbit Rings", &orbitRings);

    ImGui::Separator();

    std::vector<const char*> planetNames;
    for (auto& planet : mPlanets)
    {
        planetNames.push_back(planet.GetName());
    }

    ImGui::Combo("Select Planet", &selectedPlanet, planetNames.data(), static_cast<int>(planetNames.size()));

    ImGui::Checkbox("Lock Camera To Planet", &lockToPlanet);
    //ImGui::SliderFloat("Camera Lock DistanceY", &cameraLockDistanceY, -50.0f, 50.0f);
    ImGui::SliderFloat("Camera Lock DistanceZ", &cameraLockDistanceZ, cameraLockDistanceZMin, cameraLockDistanceZMax);

    ImGui::Checkbox("Show Planet Markers", &planetMarkers);
    ImGui::SliderFloat("Marker Scale", &markerScale, 1.0f, 100.0f);
    ImGui::SliderFloat("Marker Line Scale", &markerLineScale, 1.0f, 200.0f);

    ImGui::End();
}
void GameState::UpdateFollowCamera(float deltaTime)
{
    if (mPlanets.empty())
    {
        return;
    }

    selectedPlanet = std::clamp(selectedPlanet, 0, static_cast<int>(mPlanets.size()) - 1);

    Vector3 planetPosition = mPlanets[selectedPlanet].GetPosition();

    InputSystem* input = InputSystem::Get();

    const float turnSpeed = 2.0f;
    const float zoomSpeed = 100.0f;
    const float heightSpeed = 50.0f;

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        cameraFollowYaw += input->GetMouseMoveX() * turnSpeed * deltaTime;
        cameraFollowPitch += input->GetMouseMoveY() * turnSpeed * deltaTime;

        cameraFollowPitch = std::clamp(cameraFollowPitch, -1.2f, 1.2f);
    }

    if (input->IsKeyDown(KeyCode::W))
    {
        cameraLockDistanceZ -= zoomSpeed * deltaTime;
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        cameraLockDistanceZ += zoomSpeed * deltaTime;
    }

    if (input->IsKeyDown(KeyCode::E))
    {
        cameraLockDistanceY += heightSpeed * deltaTime;
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        cameraLockDistanceY -= heightSpeed * deltaTime;
    }

    //cameraLockDistanceZ = std::clamp(cameraLockDistanceZ, cameraLockDistanceZMin, cameraLockDistanceZMax);

    float distance = cameraLockDistanceZ;

    float cosPitch = cos(cameraFollowPitch);
    float sinPitch = sin(cameraFollowPitch);

    Vector3 cameraOffset =
    {
        sin(cameraFollowYaw) * distance * cosPitch,
        sinPitch * distance + cameraLockDistanceY,
        -cos(cameraFollowYaw) * distance * cosPitch
    };

    mCamera.SetPosition(planetPosition + cameraOffset);
    mCamera.SetLookAt(planetPosition);
}
void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? gMoveSpeed * 3 : gMoveSpeed;
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

void Planet::Initialize(const char* name, uint32_t planetSize, float rotationSpeed, float orbitRotationSpeed, float orbitDistance, float startingOrbitAngle, const char* texture, int parentIndex)
{
    float planetScaler = gPlanetSizeScaler;
    if (name == "Sun")
    {
        planetScaler = 1.0f;
    }
    mName = name;
    mMesh = MeshBuilder::CreateSpherePX(64, 64, planetSize * planetScaler);
    mMeshBuffer.Initialize(mMesh);
    mTexture = TextureManager::Get()->LoadTexture(texture);
    mRotationSpeed = rotationSpeed;
    mOrbitRotationSpeed = orbitRotationSpeed;
    mOrbitDistance = orbitDistance;
    mOrbitRotation = startingOrbitAngle;
    mParentIndex = parentIndex;
    mPlanetSize = static_cast<float>(planetSize);
}
void Planet::Terminate()
{
    TextureManager::Get()->ReleaseTexture(mTexture);
    mMeshBuffer.Terminate();
}
void Planet::Update(float deltaTime)
{
    mRotation += mRotationSpeed * deltaTime * globalSpeed;
    mOrbitRotation += mOrbitRotationSpeed * deltaTime * globalSpeed;

    float x = sin(mOrbitRotation) * mOrbitDistance;
    float z = cos(mOrbitRotation) * mOrbitDistance;

    mPosition = mParentPosition + Vector3{ x, 0.0f, z };
}
void Planet::Render(Camera& mCamera, ConstantBuffer& mConstantBuffer)
{
    Matrix4 matRot = Matrix4::RotationY(mRotation);
    Matrix4 matTrans = Matrix4::Translation(mPosition);
    Matrix4 matWorld = matRot * matTrans;
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    Math::Matrix4 wvp = matWorld * matView * matProj;
    wvp = Math::Transpose(wvp);
    mConstantBuffer.Update(&wvp);
    TextureManager::Get()->BindPS(mTexture, 0);

    mMeshBuffer.Render();
}
void Planet::SetParentPosition(const Vector3& parentPosition)
{
    mParentPosition = parentPosition;
}
Vector3 Planet::GetPosition() const
{
    return mPosition;
}
float Planet::GetOrbitDistance() const
{
    return mOrbitDistance;
}
int Planet::GetParentIndex() const
{
    return mParentIndex;
}
const char* Planet::GetName() const
{
    return mName.c_str();
}
float Planet::GetPlanetSize() const
{
    return mPlanetSize;
}