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

	GomiEngine::Graphics::MeshPC mMesh;

	GomiEngine::Graphics::MeshBuffer mMeshBuffer;
	GomiEngine::Graphics::VertexShader mVertexShader;
	GomiEngine::Graphics::PixelShader mPixelShader;

	GomiEngine::Graphics::Camera mCamera;
	GomiEngine::Graphics::ConstantBuffer mConstantBuffer;
};