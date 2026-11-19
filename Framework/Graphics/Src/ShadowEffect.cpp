#include "Precompiled.h"
#include "ShadowEffect.h"
#include "VertexTypes.h"
#include "RenderObject.h"
#include "DirectionalLight.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;


void ShadowEffect::Initialize()
{
    std::filesystem::path shaderPath = L"../../Assets/Shaders/Shadow.fx";
    mVertexShader.Initialize<Vertex>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mTransformBuffer.Initialize();

    mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);
    mLightCamera.SetNearPlane(1.0f);
    mLightCamera.SetFarPlane(2000.0f);

    constexpr uint32_t depthMapResolution = 4096;
    mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, RenderTarget::Format::RGBA_U32);
}
void ShadowEffect::Terminate()
{
    mDepthMapRenderTarget.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}
void ShadowEffect::Begin()
{
    UpdateLightCamera();

    mVertexShader.Bind();
    mPixelShader.Bind();
    mTransformBuffer.BindVS(0);

    mDepthMapRenderTarget.BeginRender();
}
void ShadowEffect::End()
{
    mDepthMapRenderTarget.EndRender();
}
void ShadowEffect::Render(const RenderObject& renderObject)
{
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
    const Math::Matrix4 matView = mLightCamera.GetViewMatrix();
    const Math::Matrix4 matProj = mLightCamera.GetProjectionMatrix();

    TransformData data;
    data.wvp = Math::Transpose(matWorld * matView * matProj);
    mTransformBuffer.Update(data);

    renderObject.meshBuffer.Render();
}
void ShadowEffect::Render(const RenderGroup& renderGroup)
{
    const Math::Matrix4 matWorld = renderGroup.transform.GetMatrix();
    const Math::Matrix4 matView = mLightCamera.GetViewMatrix();
    const Math::Matrix4 matProj = mLightCamera.GetProjectionMatrix();

    TransformData data;
    data.wvp = Math::Transpose(matWorld * matView * matProj);
    mTransformBuffer.Update(data);

    for(const RenderObject& renderObject : renderGroup.renderObjects)
    {
        //const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
        //data.wvp = Math::Transpose(matWorld * matView * matProj);
        //mTransformBuffer.Update(data);
        renderObject.meshBuffer.Render();
    }
}
void ShadowEffect::DebugUI()
{
    if(ImGui::CollapsingHeader("Shadow Effect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("DepthMap");
        ImGui::Image(
            mDepthMapRenderTarget.GetRawData(),
            { 144, 144 },
            { 0, 0 },
            { 1, 1 },
            { 1, 1, 1, 1 },
            { 1, 1, 1, 1 });
        ImGui::DragFloat("Size##ShadowEffect", &mSize, 1.0f, 1.0f, 1000.0f);
        ImGui::DragFloat3("FocusPoint##ShadowEffect", &mFocusPoint.x);
    }
}
void ShadowEffect::SetDirectionaLight(const DirectionalLight& directionaLight)
{
    mDirectionaLight = &directionaLight;
}
void ShadowEffect::SetFOcus(const Math::Vector3& focusPoint)
{
    mFocusPoint = focusPoint;
}
void ShadowEffect::SetSize(float size)
{
    mSize = size;
}
const Camera& ShadowEffect::GetLightCamera() const
{
    return mLightCamera;
}
const Texture& ShadowEffect::GetDepthMap() const
{
    return mDepthMapRenderTarget;
}
void ShadowEffect::UpdateLightCamera()
{
    ASSERT(mDirectionaLight != nullptr, "ShadowEffect: Directional light is not set");
    const Math::Vector3 direction = mDirectionaLight->direction;
    mLightCamera.SetDirection(direction);
    mLightCamera.SetPosition(mFocusPoint - direction * 1000.0f);
    mLightCamera.SetSize(mSize, mSize);
}