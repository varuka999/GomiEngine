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
    TextureManager* tm = TextureManager::Get();
    mRenderObject.diffuseMapId = tm->LoadTexture("planets/earth/earth.jpg");
    mRenderObject.specMapId = tm->LoadTexture("planets/earth/earth_spec.jpg");
    mRenderObject.normMapId = tm->LoadTexture("planets/earth/earth_normal.jpg");
    mRenderObject.bumpMapId = tm->LoadTexture("planets/earth/earth_bump.jpg");

    // set up 2nd render object
    Mesh mesh2 = MeshBuilder::CreateSphere(30, 30, 1.0f);
    mRenderObject2.meshBuffer.Initialize(mesh2);
    mRenderObject2.transform.position = { -2.5f, 0.0f, 2.0f };
    mRenderObject2.diffuseMapId = tm->LoadTexture("mountain/mountain_texture.jpg");
    mRenderObject2.specMapId = tm->LoadTexture("mountain/mountain_spec.jpg");
    mRenderObject2.normMapId = tm->LoadTexture("mountain/mountain_normal.jpg");
    mRenderObject2.bumpMapId = tm->LoadTexture("mountain/mountain_height.jpg"); // not bump but it kinda looked funny

    // set up 3rdrender object
    Mesh mesh3 = MeshBuilder::CreateSphere(30, 30, 1.0f);
    mRenderObject3.meshBuffer.Initialize(mesh3);
    mRenderObject3.transform.position = { 2.5f, 0.0f, 2.0f };
    mRenderObject3.diffuseMapId = tm->LoadTexture("water/water_texture.jpg");
    mRenderObject3.specMapId = tm->LoadTexture("water/water_spec.jpg");
    mRenderObject3.normMapId = tm->LoadTexture("water/water_normal.jpg");
    mRenderObject3.bumpMapId = tm->LoadTexture("water/water_height.jpg");

    // set up the effect
    std::filesystem::path shaderPath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderPath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    // set up cel effect
    shaderPath = L"../../Assets/Shaders/CelShader.fx";
    mStandardEffectCel.Initialize(shaderPath);
    mStandardEffectCel.SetCamera(mCamera);
    mStandardEffectCel.SetDirectionalLight(mDirectionalLight);

    // initialize render target
    const uint32_t size = 512;
    mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);
}
void GameState::Terminate()
{
    mRenderTarget.Terminate();
    mStandardEffectCel.Terminate();
    mStandardEffect.Terminate();
    mRenderObject3.Terminate();
    mRenderObject2.Terminate();
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

       if (mUseCellShading)
       {
           mStandardEffectCel.Begin();
           mStandardEffectCel.Render(mRenderObject);
           mStandardEffectCel.Render(mRenderObject2);
           mStandardEffectCel.Render(mRenderObject3);
           mStandardEffectCel.End();
       }
       else
       {
           mStandardEffect.Begin();
           mStandardEffect.Render(mRenderObject);
           mStandardEffect.Render(mRenderObject2);
           mStandardEffect.Render(mRenderObject3);
           mStandardEffect.End();
       }

       // render target render
       mRenderTarget.BeginRender(); // tell gpu to render here
       mStandardEffect.Begin();
       mStandardEffect.Render(mRenderObject);
       mStandardEffect.Render(mRenderObject2);
       mStandardEffect.Render(mRenderObject3);
       mStandardEffect.End();
       mRenderTarget.EndRender();
}
void GameState::DebugUI() // tell gpu to render to window again
{
    ImGui::Begin("Hello Lighting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }
        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
    }
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);
        ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
        ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
        ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
        ImGui::DragFloat("Shininess##Material", &mRenderObject.material.shininess, 0.01f, 0.0f, 1000.0f);
    }
    if (ImGui::CollapsingHeader("Material 2", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ColorEdit4("Emissive##Material2", &mRenderObject2.material.emissive.r);
        ImGui::ColorEdit4("Ambient##Material2", &mRenderObject2.material.ambient.r);
        ImGui::ColorEdit4("Diffuse##Material2", &mRenderObject2.material.diffuse.r);
        ImGui::ColorEdit4("Specular##Material2", &mRenderObject2.material.specular.r);
        ImGui::DragFloat("Shininess##Material2", &mRenderObject2.material.shininess, 0.01f, 0.0f, 1000.0f);
    }
    if (ImGui::CollapsingHeader("Material 3", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::ColorEdit4("Emissive##Material3", &mRenderObject3.material.emissive.r);
        ImGui::ColorEdit4("Ambient##Material3", &mRenderObject3.material.ambient.r);
        ImGui::ColorEdit4("Diffuse##Material3", &mRenderObject3.material.diffuse.r);
        ImGui::ColorEdit4("Specular##Material3", &mRenderObject3.material.specular.r);
        ImGui::DragFloat("Shininess##Material3", &mRenderObject3.material.shininess, 0.01f, 0.0f, 1000.0f);
    }
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 });
    ImGui::Checkbox("UseCelShader", &mUseCellShading);
    if (mUseCellShading)
    {
        mStandardEffectCel.DebugUI();
    }
    else
    {
        mStandardEffect.DebugUI();
    }
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
