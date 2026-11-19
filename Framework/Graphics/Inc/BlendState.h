#pragma once


namespace GomiEngine::Graphics
{
    class BlendState
    {
    public:
        enum class Mode
        {
            Opaque,
            AlphaBlend,
            AlphaPremultiplied,
            Additive,
        };

        static void ClearState();

        BlendState() = default;
        ~BlendState();

        BlendState(const BlendState&) = delete;
        BlendState& operator=(const BlendState&) = delete;

        void Initialize(Mode mode);
        void Terminate();
        void Set() const;

    private:
        ID3D11BlendState* mBlendState = nullptr;
        ID3D11DepthStencilState* mDepthStencilState = nullptr;
    };
}