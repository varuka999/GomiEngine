#pragma once

namespace GomiEngine::Graphics
{
	class VertexShader final
	{
	public:
		template<class VertexType>
		void Initialize(const std::filesystem::path& shaderPath)
		{
			Initialize(shaderPath, VertexType::Format);
		}
		void Initialize(const std::filesystem::path& shaderPath, uint32_t format);
		void Terminate();
		void Bind();
	private:
		// what functions are called on the shader file per vertex
		ID3D11VertexShader* mVertexShader = nullptr;
		// what data am I expecting on the shader (position, color, texture...)
		ID3D11InputLayout* mInputLayout = nullptr;
	};
}