#pragma once

#include <GomiEngine/Inc/GomiEngine.h>

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Math;


class Planet
{
public:
    void Initialize(uint32_t planetSize, float rotationSpeed, float orbitRotation, float orbitDistance, std::string texture, Planet* parent = nullptr);
    void Terminate();
    void Update(float deltaTime);
    void Render(Camera &mCamera, ConstantBuffer &mConstantBuffer);

    Matrix4 GetMatrixWorld();

private:
    MeshPX mMesh;
    MeshBuffer mMeshBuffer;
    TextureId mTexture;
    Vector3 mPosition;
    
    Planet* mParentPlanet;
    float mRotation;
    float mRotationSpeed;
    float mOrbitRotation;
    float mOrbitRotationSpeed;
    float mOrbitDistance;
    Matrix4 mMatWorld;
};

class GameState : public AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;
private:
    void PlanetRenders();
    void UpdateCamera(float deltaTime);

    MeshPX mSpacePX;
    MeshPX mSunPX;
    MeshPX mMercuryPX;
    MeshPX mVenusPX;
    MeshPX mEarthPX;
    MeshPX mMarsPX;

    Vector3 mSunPosition;
    Vector3 mMercuryPosition;
    Vector3 mVenusPosition;
    Vector3 mEarthPosition;
    Vector3 mMarsPosition;

    MeshBuffer mSpaceMeshBuff;
    MeshBuffer mSunMeshBuff;
    MeshBuffer mMercuryMeshBuff;
    MeshBuffer mVenusMeshBuff;
    MeshBuffer mEarthMeshBuff;
    MeshBuffer mMarsMeshBuff;

    TextureId mSpaceSkyboxTexture;
    TextureId mSunTexture;
    TextureId mMercuryTexture;
    TextureId mVenusTexture;
    TextureId mEarthTexture;
    TextureId mMarsTexture;

    Planet mSunPlanet;
    Planet mEarthPlanet;
    Planet mEarthPlanet2;
    
    VertexShader mVertexShader;
    PixelShader mPixelShader;
    Sampler mSampler;

    Camera mCamera;
    ConstantBuffer mConstantBuffer;
};
