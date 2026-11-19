#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Camera.h"
#include "RenderTarget.h"

namespace GomiEngine::Graphics
{
    class RenderObject;
    class RenderGroup;
    struct DirectionalLight;

    class ShadowEffect final
    {
    public:
        void Initialize();
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);
        void Render(const RenderGroup& renderGroup);

        void DebugUI();

        void SetDirectionaLight(const DirectionalLight& directionaLight);
        void SetFOcus(const Math::Vector3& focusPoint);
        void SetSize(float size);
        const Camera& GetLightCamera() const;
        const Texture& GetDepthMap() const;

    private:
        void UpdateLightCamera();

        struct TransformData
        {
            Math::Matrix4 wvp;
        };
        using TransformBuffer = TypedConstantBuffer<TransformData>;
        TransformBuffer mTransformBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;

        Camera mLightCamera;
        RenderTarget mDepthMapRenderTarget;

        const DirectionalLight* mDirectionaLight = nullptr;
        Math::Vector3 mFocusPoint = Math::Vector3::Zero;
        float mSize = 10.0f;
    };
}