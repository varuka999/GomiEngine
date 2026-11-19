#include "Precompiled.h"
#include "BlendState.h"

#include "GraphicsSystem.h"

using namespace GomiEngine;
using namespace GomiEngine::Graphics;

namespace
{
    D3D11_BLEND GetSrcBlend(BlendState::Mode mode)
    {
        switch (mode)
        {
        case BlendState::Mode::Additive:            return D3D11_BLEND_SRC_ALPHA;
        case BlendState::Mode::AlphaBlend:          return D3D11_BLEND_SRC_ALPHA;
        case BlendState::Mode::AlphaPremultiplied:  return D3D11_BLEND_ONE;
        case BlendState::Mode::Opaque:              return D3D11_BLEND_ONE;
        default:
            ASSERT(false, "BlendState: blend mode not found");
            break;
        }
        return D3D11_BLEND_ONE;
    }
    D3D11_BLEND GetDestBlend(BlendState::Mode mode)
    {
        switch (mode)
        {
        case BlendState::Mode::Additive:            return D3D11_BLEND_ONE;
        case BlendState::Mode::AlphaBlend:          return D3D11_BLEND_INV_SRC_ALPHA;
        case BlendState::Mode::AlphaPremultiplied:  return D3D11_BLEND_INV_SRC_ALPHA;
        case BlendState::Mode::Opaque:              return D3D11_BLEND_ZERO;
        default:
            ASSERT(false, "BlendState: blend mode not found");
            break;
        }
        return D3D11_BLEND_ZERO;
    }
}

void BlendState::ClearState()
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->OMSetBlendState(nullptr, nullptr, UINT_MAX);
    context->OMSetDepthStencilState(nullptr, 0);
}
BlendState::~BlendState()
{
    ASSERT(mBlendState == nullptr, "BlendState: terminate must be called");

}
void BlendState::Initialize(Mode mode)
{

}
void BlendState::Terminate()
{
    SafeRelease(mBlendState);
    SafeRelease(mDepthStencilState);
}
void BlendState::Set() const
{
    auto context = GraphicsSystem::Get()->GetContext();
    context->OMSetBlendState(mBlendState, nullptr, UINT_MAX);
    context->OMSetDepthStencilState(mDepthStencilState, 0);
}