#pragma once

#include <GomiEngine/Inc/GomiEngine.h>

class ShapeStates : public GomiEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
protected:
	virtual void CreateShapes();
	virtual void Move(float deltaTime);

	GomiEngine::Graphics::TextureId mTextureId;
	GomiEngine::Graphics::MeshPX mMesh1;
	GomiEngine::Graphics::MeshPX mMesh2;
	GomiEngine::Graphics::MeshPX mMesh3;
	GomiEngine::Graphics::MeshPX mMesh4;

	GomiEngine::Graphics::MeshBuffer mMeshBuffer1;
	GomiEngine::Graphics::MeshBuffer mMeshBuffer2;
	GomiEngine::Graphics::MeshBuffer mMeshBuffer3;
	GomiEngine::Graphics::MeshBuffer mMeshBuffer4;
	GomiEngine::Graphics::VertexShader mVertexShader;
	GomiEngine::Graphics::PixelShader mPixelShader;
	GomiEngine::Graphics::Sampler mSampler;

	GomiEngine::Graphics::Camera mCamera;
	GomiEngine::Graphics::ConstantBuffer mConstantBuffer;
	GomiEngine::Math::Vector3 mPosition1;
	GomiEngine::Math::Vector3 mPosition2;
	GomiEngine::Math::Vector3 mPosition3;
	GomiEngine::Math::Vector3 mPosition4;
};

class VertexCubeState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class CubeState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class CubePXState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class BoxState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class PyramidState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class PlaneState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class CylinderState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class SphereState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class SphereXState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
class SkySphereXState : public ShapeStates
{
public:
	void Update(float deltaTime);
protected:
	void CreateShapes() override;
};
