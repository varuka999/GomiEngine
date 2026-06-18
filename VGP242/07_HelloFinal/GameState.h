#pragma once

#include <GomiEngine/Inc/GomiEngine.h>

using namespace GomiEngine;
using namespace GomiEngine::Graphics;
using namespace GomiEngine::Math;

class Planet
{
public:
    void Initialize(
        const char* name,
        uint32_t planetSize,
        float rotationSpeed,
        float orbitRotationSpeed,
        float orbitDistance,
        const char* texture = "planets/pluto.jpg",
        float startingOrbitAngle = 0.0f,
        int parentIndex = -1,
        float axisTiltDegrees = 0.0f,
        float orbitTiltDegrees = 0.0f
    );

    void Terminate();
    void Update(float deltaTime);
    void Render(Camera& mCamera, ConstantBuffer& mConstantBuffer);

    void SetParentPosition(const Vector3& parentPosition);
    Vector3 GetPosition() const;
    float GetOrbitDistance() const;
    int GetParentIndex() const;
    const char* GetName() const;
    float GetPlanetSize() const;
    float GetOrbitTilt() const;
    float GetAxisTilt() const;

private:
    std::string mName;
    MeshPX mMesh;
    MeshBuffer mMeshBuffer;
    TextureId mTexture;
    Vector3 mPosition;
    Vector3 mParentPosition;
    int mParentIndex = -1;

    float mRotation = 0.0f;
    float mRotationSpeed = 0.0f;
    float mOrbitRotation = 0.0f;
    float mOrbitRotationSpeed = 0.0f;
    float mOrbitDistance = 0.0f;
    float mOrbitTilt = 0.0f;
    float mAxisTilt = 0.0f;
    float mPlanetSize = 1.0f;
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
    void DrawOrbitRings();
    void DrawPlanetMarkers();
    void DrawTiltedOrbitCircle(uint32_t slices, float radius, const Vector3& origin, float orbitTilt, const Color& color);
    void UpdateFollowCamera(float deltaTime);
    void UpdateCamera(float deltaTime);

    std::vector<Planet> mPlanets;

    MeshPX mSpacePX;
    MeshBuffer mSpaceMeshBuff;
    TextureId mSpaceSkyboxTexture;
    
    VertexShader mVertexShader;
    PixelShader mPixelShader;
    Sampler mSampler;

    Camera mCamera;
    ConstantBuffer mConstantBuffer;
};
