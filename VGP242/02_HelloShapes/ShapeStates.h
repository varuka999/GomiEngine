#pragma once

#include <GomiEngine/Inc/GomiEngine.h>

class ShapeState : public GomiEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
protected:
    virtual void CreateShape();

    // data to create an object
    struct Vertex
    {
        GomiEngine::Math::Vector3 position;
    };
    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    // info to pass to the gpu
    // stores the date for the shape
    ID3D11Buffer* mVertexBuffer = nullptr;
    // what functions are called on the shader file per vertex
    ID3D11VertexShader* mVertexShader = nullptr;
    // what data am i expecting on the shader(position, color, texture...)
    ID3D11InputLayout* mInputLayout = nullptr;
    // what functions are called on the shader filed per pixel
    ID3D11PixelShader* mPixelShader = nullptr;
};