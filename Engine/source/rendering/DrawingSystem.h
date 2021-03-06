#pragma once
#include "System.h"
#include "GraphicsDeviceInterface.h"
#include "Shader.h"
#include "components/Components.h"
#include "RenderTexture.h"

namespace Baryon
{
class VirtualScreen;

class DrawingSystem : public System<WorldMatrixComponent, MeshComponent>, GraphicsDeviceInterface
{
	using super = System<WorldMatrixComponent, MeshComponent>;
public:
	DrawingSystem(EntityManager* entityManager, std::vector<VirtualScreen>& virtualScreens);
	void initialize() override;
	void terminate() override;
	void tick() override;
private:
	void update(WorldMatrixComponent& wmc, MeshComponent& mesh) override;

	std::vector<VirtualScreen>& virtualScreens;

	VertexShader vs{L"../../Engine/shaders/VertexShader.hlsl"};
	PixelShader ps{L"../../Engine/shaders/PixelShader.hlsl"};

	PixelShader TAA{ L"../../Engine/shaders/AntiAliasing.hlsl" };
	VertexShader postVS{L"../../Engine/shaders/FullscreenVS.hlsl"};
	PixelShader lightPS{L"../../Engine/shaders/LightPS.hlsl"};
	PixelShader postPS{L"../../Engine/shaders/PostProcessPS.hlsl"};
};
}
