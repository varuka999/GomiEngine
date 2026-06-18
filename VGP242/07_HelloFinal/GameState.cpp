#include "GameState.h"
#include <algorithm>

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Input;

int gPlanetSizeScaler = 1.0f;

// ImGUI
float globalSpeed = 10.0f;
bool orbitRings = true;
int selectedPlanet = 3;
bool lockToPlanet = false;
float gMoveSpeed = 5000.0f;
float cameraLockDistanceY = 0.0f;
float cameraLockDistanceZ = 500.0f;
float cameraLockDistanceZMin = 10.0f;
float cameraLockDistanceZMax = 5000.0f;
float cameraFollowYaw = 0.0f;
float cameraFollowPitch = 0.25f;

bool planetMarkers = false;
bool axisMarkers = true;
float markerScale = 10.0f;
float markerLineScale = 20.0f;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 10000.0f, -20000.0f });
    mCamera.SetLookAt({ 0.0f, -135.0f, 0.0f });

    mCamera.SetNearPlane(1.0f);
    mCamera.SetFarPlane(1e7f);

    mSpacePX = MeshBuilder::CreateSkySpherePX(1280, 1280, 800000);
    mSpaceMeshBuff.Initialize(mSpacePX);

    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mConstantBuffer.Initialize(sizeof(Math::Matrix4));
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    mSpaceSkyboxTexture = TextureManager::Get()->LoadTexture("skysphere/space.jpg");

    mPlanets.reserve(64);

    mPlanets.emplace_back(); mPlanets.back().Initialize("Sun", 1391, 0.040f, 0.0f, 0.0f, "planets/sun.jpg", 0.0f, -1, 7.25f, 0.0f);

    mPlanets.emplace_back(); mPlanets.back().Initialize("Mercury", 5, 0.017f, 0.01137f, 5790.0f, "planets/mercury.jpg", 1.0f, 0, 0.01f, 7.0f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Venus", 12, -0.004f, 0.00445f, 10800.0f, "planets/venus.jpg", 5.0f, 0, 177.4f, 3.4f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Earth", 13, 1.000f, 0.00274f, 14900.0f, "planets/earth/earth.jpg", 24.0f, 0, 23.4f, 0.0f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Moon", 3, 0.037f, 0.03660f, 384.4f, "planets/pluto.jpg", 65.0f, 3, 6.7f, 5.1f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Mars", 7, 0.972f, 0.00146f, 22790.0f, "planets/mars.jpg", 100.0f, 0, 25.2f, 1.9f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Jupiter", 143, 2.412f, 0.000231f, 77860.0f, "planets/jupiter.jpg", 123.0f, 0, 3.1f, 1.3f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Saturn", 130, 2.246f, 0.000093f, 143350.0f, "planets/saturn.jpg", 154.0f, 0, 26.7f, 2.5f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Uranus", 51, -1.388f, 0.0000326f, 287250.0f, "planets/uranus.jpg", 190.0f, 0, 97.8f, 0.8f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Neptune", 49, 1.486f, 0.0000166f, 449510.0f, "planets/neptune.jpg", 250.0f, 0, 28.3f, 1.8f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Pluto", 2, -0.156f, 0.0000110f, 587000.0f, "planets/pluto.jpg", 290.0f, 0, 122.5f, 17.2f);

    // Mars moons
    mPlanets.emplace_back(); mPlanets.back().Initialize("Phobos", 1, 1.0f, 3.136f, 9.4f, "planets/pluto.jpg", 0.3f, 5, 0.0f, 1.1f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Deimos", 1, 1.0f, 0.792f, 23.5f, "planets/pluto.jpg", 2.1f, 5, 0.0f, 1.8f);

    // Jupiter moons
    mPlanets.emplace_back(); mPlanets.back().Initialize("Io", 4, 1.0f, 0.565f, 421.7f, "planets/pluto.jpg", 0.5f, 6, 0.0f, 0.04f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Europa", 3, 1.0f, 0.282f, 671.1f, "planets/pluto.jpg", 1.4f, 6, 0.0f, 0.47f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Ganymede", 5, 1.0f, 0.140f, 1070.4f, "planets/pluto.jpg", 2.6f, 6, 0.0f, 0.20f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Callisto", 5, 1.0f, 0.060f, 1882.7f, "planets/pluto.jpg", 4.1f, 6, 0.0f, 0.28f);

    // Saturn moons
    mPlanets.emplace_back(); mPlanets.back().Initialize("Mimas", 1, 1.0f, 1.064f, 185.5f, "planets/pluto.jpg", 0.0f, 7, 0.0f, 1.57f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Enceladus", 1, 1.0f, 0.730f, 238.0f, "planets/pluto.jpg", 0.8f, 7, 0.0f, 0.01f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Tethys", 1, 1.0f, 0.529f, 294.7f, "planets/pluto.jpg", 1.6f, 7, 0.0f, 1.12f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Dione", 1, 1.0f, 0.365f, 377.4f, "planets/pluto.jpg", 2.4f, 7, 0.0f, 0.02f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Rhea", 2, 1.0f, 0.221f, 527.1f, "planets/pluto.jpg", 3.2f, 7, 0.0f, 0.35f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Titan", 5, 1.0f, 0.063f, 1221.9f, "planets/pluto.jpg", 4.0f, 7, 0.0f, 0.33f);
    mPlanets.emplace_back(); mPlanets.back().Initialize("Iapetus", 1, 1.0f, 0.0126f, 3560.8f, "planets/pluto.jpg", 4.8f, 7, 0.0f, 15.47f);

    // Neptune moon
    mPlanets.emplace_back(); mPlanets.back().Initialize("Triton", 3, 1.0f, -0.170f, 354.8f, "planets/pluto.jpg", 1.2f, 9, 0.0f, 157.0f);

    // Pluto moon
    mPlanets.emplace_back(); mPlanets.back().Initialize("Charon", 1, 1.0f, 0.157f, 19.6f, "planets/pluto.jpg", 0.7f, 10, 0.0f, 0.0f);
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

    if (planetMarkers || axisMarkers)
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

        DrawTiltedOrbitCircle(256, planet.GetOrbitDistance(), center, planet.GetOrbitTilt(), Colors::White);
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
        Matrix4 matAxisTilt = Matrix4::RotationZ(planet.GetAxisTilt());
        Vector3 axisDirection = Math::TransformNormal(Vector3::YAxis, matAxisTilt);

        axisDirection = Math::Normalize(axisDirection);

        if (planetMarkers)
        {
            SimpleDraw::AddSphere(16, 16, markerRadius, position, Colors::White);
        }

        if (axisMarkers)
        {
            SimpleDraw::AddLine(position - axisDirection * markerLineHeight, position, Colors::Yellow);

            SimpleDraw::AddLine(position, position + axisDirection * markerLineHeight, Colors::Red);
        }
    }
}
void GameState::DrawTiltedOrbitCircle(uint32_t slices, float radius, const Vector3& origin, float orbitTilt, const Color& color)
{
    Matrix4 matOrbitTilt = Matrix4::RotationX(orbitTilt);

    for (uint32_t i = 0; i < slices; ++i)
    {
        float t0 = static_cast<float>(i) / static_cast<float>(slices);
        float t1 = static_cast<float>(i + 1) / static_cast<float>(slices);

        float angle0 = t0 * Math::Constants::TwoPi;
        float angle1 = t1 * Math::Constants::TwoPi;

        Vector3 p0 =
        {
            sin(angle0) * radius,
            0.0f,
            cos(angle0) * radius
        };

        Vector3 p1 =
        {
            sin(angle1) * radius,
            0.0f,
            cos(angle1) * radius
        };

        p0 = Math::TransformNormal(p0, matOrbitTilt);
        p1 = Math::TransformNormal(p1, matOrbitTilt);

        SimpleDraw::AddLine(origin + p0, origin + p1, color);
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Solar System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SliderFloat("Move Speed", &gMoveSpeed, 1000.0f, 10000.0f);
    ImGui::SliderFloat("Orbit & Rotation Speed", &globalSpeed, 1.0f, 1000.0f);
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

    ImGui::Checkbox("Show Planet Sphere", &planetMarkers);
    ImGui::Checkbox("Show Planet Axis", &axisMarkers);
    ImGui::SliderFloat("Sphere Scale", &markerScale, 1.5f, 100.0f);
    ImGui::SliderFloat("Axis Line Scale", &markerLineScale, 1.5f, 100.0f);

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

void Planet::Initialize(
    const char* name,
    uint32_t planetSize,
    float rotationSpeed,
    float orbitRotationSpeed,
    float orbitDistance,
    const char* texture,
    float startingOrbitAngle,
    int parentIndex,
    float axisTiltDegrees,
    float orbitTiltDegrees)
{
    float planetScaler = static_cast<float>(gPlanetSizeScaler);

    if (std::strcmp(name, "Sun") == 0)
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

    mAxisTilt = axisTiltDegrees * Math::Constants::DegToRad;
    mOrbitTilt = orbitTiltDegrees * Math::Constants::DegToRad;
}
void Planet::Terminate()
{
    TextureManager::Get()->ReleaseTexture(mTexture);
    mMeshBuffer.Terminate();
}
void Planet::Update(float deltaTime)
{
    mRotation += -mRotationSpeed * deltaTime * globalSpeed;
    mOrbitRotation += -mOrbitRotationSpeed * deltaTime * globalSpeed;

    Vector3 orbitOffset =
    {
        sin(mOrbitRotation) * mOrbitDistance,
        0.0f,
        cos(mOrbitRotation) * mOrbitDistance
    };

    Matrix4 matOrbitTilt = Matrix4::RotationX(mOrbitTilt);
    orbitOffset = Math::TransformCoord(orbitOffset, matOrbitTilt);

    mPosition = mParentPosition + orbitOffset;
}
void Planet::Render(Camera& mCamera, ConstantBuffer& mConstantBuffer)
{
    Matrix4 matSelfRot = Matrix4::RotationY(mRotation);
    Matrix4 matAxisTilt = Matrix4::RotationZ(mAxisTilt);
    Matrix4 matTrans = Matrix4::Translation(mPosition);

    Matrix4 matWorld = matSelfRot * matAxisTilt * matTrans;

    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();

    Matrix4 wvp = matWorld * matView * matProj;
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
float Planet::GetOrbitTilt() const
{
    return mOrbitTilt;
}
float Planet::GetAxisTilt() const
{
    return mAxisTilt;
}