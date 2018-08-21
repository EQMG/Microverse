#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSinwave.hpp>
#include <Meshes/RendererMeshes.hpp>
#include <Fonts/RendererFonts.hpp>
#include <Guis/RendererGuis.hpp>
#include <Particles/RendererParticles.hpp>
#include <Shadows/RendererShadows.hpp>
#include <Post/Deferred/RendererDeferred.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Post/Filters/FilterTiltshift.hpp>
#include <Fog/RendererFog.hpp>
#include "Post/Filters/FilterDamage.hpp"

namespace test
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
	{
		4096, 4096, // width, height
		{
			Attachment(0, ATTACHMENT_IMAGE, VK_FORMAT_R8_UNORM) // shadows
		}, // images
		{
			SubpassType(0, {0})
		} // subpasses
	};
	RenderpassCreate *RENDERPASS_1_CREATE = new RenderpassCreate
	{
		0, 0, // width, height
		{
			Attachment(0, ATTACHMENT_DEPTH, VK_FORMAT_D32_SFLOAT_S8_UINT, true), // depth
			Attachment(1, ATTACHMENT_SWAPCHAIN), // swapchain
			Attachment(2, ATTACHMENT_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT, true), // positions (world-space)
			Attachment(3, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, true), // diffuse
			Attachment(4, ATTACHMENT_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT, true), // normals (world-space)
			Attachment(5, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, true), // materials
			Attachment(6, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM) // resolved
		}, // images
		{
			SubpassType(0, {0, 2, 3, 4, 5}),
			SubpassType(1, {0, 6}),
			SubpassType(2, {1})
		} // subpasses
	};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE})
	{
	//	AddRenderer<RendererShadows>(GraphicsStage(0, 0));
	
		AddRenderer<RendererMeshes>(GraphicsStage(1, 0));
	//	AddRenderer<RendererParticles>(GraphicsStage(1, 0));
		AddRenderer<RendererDeferred>(GraphicsStage(1, 1));
		AddRenderer<RendererFog>(GraphicsStage(1, 1));
		AddRenderer<FilterDefault>(GraphicsStage(1, 2));
	//	AddRenderer<FilterFxaa>(GraphicsStage(1, 2));
	//	AddRenderer<FilterLensflare>(GraphicsStage(1, 2));
	//	AddRenderer<FilterTiltshift>(GraphicsStage(1, 2));
	//	AddRenderer<FilterDamage>(GraphicsStage(1, 2));
	//	AddRenderer<FilterGrain>(GraphicsStage(1, 2));
		AddRenderer<RendererGuis>(GraphicsStage(1, 2));
		AddRenderer<RendererFonts>(GraphicsStage(1, 2));

#ifdef ACID_BUILD_MACOS
		GetRenderer<FilterFxaa>()->SetEnabled(false);
#endif
	}

	MainRenderer::~MainRenderer()
	{
	}

	void MainRenderer::Update()
	{
		RENDERPASS_0_CREATE->SetWidth(Shadows::Get()->GetShadowSize());
		RENDERPASS_0_CREATE->SetHeight(Shadows::Get()->GetShadowSize());

		/*auto filterDamage = GetRenderer<FilterDamage>();

		if (filterDamage != nullptr)
		{
			filterDamage->SetColour(Colour::RED);
			filterDamage->SetRadiusDriver<DriverConstant>(0.55f);
			filterDamage->SetSoftnessDriver<DriverSinwave>(0.25f, 0.35f, 2.0f);
		}*/
	}
}
