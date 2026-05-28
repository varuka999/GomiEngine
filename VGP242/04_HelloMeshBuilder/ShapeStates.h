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

	GomiEngine::Graphics::MeshPC mMesh;

	GomiEngine::Graphics::MeshBuffer mMeshBuffer;
	GomiEngine::Graphics::VertexShader mVertexShader;
	GomiEngine::Graphics::PixelShader mPixelShader;

	GomiEngine::Graphics::Camera mCamera;
	GomiEngine::Graphics::ConstantBuffer mConstantBuffer;
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
