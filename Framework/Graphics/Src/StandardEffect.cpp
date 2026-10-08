#include "Precompiled.h"
#include "StandardEffect.h"

#include "Camera.h"
#include "VertexTypes.h"
#include "RenderObject.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& shaderPath)
{
    // initialize buffers
    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();

    // other rendering components
    mVertexShader.Initialize<Vertex>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}
void StandardEffect::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
}
void StandardEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindVS(0);
    mSampler.BindPS(0);

    mTransformBuffer.BindVS(0);
    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);
    mMaterialBuffer.BindPS(2);
}
void StandardEffect::End()
{
    // come back for shadows
}
void StandardEffect::Render(const RenderObject& renderObject)
{
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData data;
    data.wvp = Math::Transpose(matFinal);
    mTransformBuffer.Update(data);
    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    renderObject.meshBuffer.Render();
}
void StandardEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // later
    }
}
void StandardEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}
void StandardEffect::SetDirectionalLight(const DirectionalLight& directionaLight)
{
    mDirectionalLight = &directionaLight;
}