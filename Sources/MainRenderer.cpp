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
#include "Post/Filters/FilterDamage.hpp"

namespace micro
{
	const RenderpassCreate RENDERPASS_0_CREATE = RenderpassCreate
	{
		4096, 4096, // width, height
		{
			Attachment(0, ATTACHMENT_IMAGE, VK_FORMAT_R8_UNORM) // shadows
		}, // images
		{
			SubpassType(0, {0})
		} // subpasses
	};
	const RenderpassCreate RENDERPASS_1_CREATE = RenderpassCreate
	{
		0, 0, // width, height
		{
			Attachment(0, ATTACHMENT_DEPTH, VK_FORMAT_D32_SFLOAT_S8_UINT, false), // depth
			Attachment(1, ATTACHMENT_SWAPCHAIN), // swapchain
			Attachment(2, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false), // diffuse
			Attachment(3, ATTACHMENT_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT, false), // normals
			Attachment(4, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false), // materials
			Attachment(5, ATTACHMENT_IMAGE, VK_FORMAT_R8G8B8A8_UNORM) // resolved
		}, // images
		{
			SubpassType(0, {0, 2, 3, 4}),
			SubpassType(1, {0, 5}),
			SubpassType(2, {1})
		} // subpasses
	};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE})
	{
	}

	void MainRenderer::Start()
	{
	//	Renderer::Get()->AddRenderer<RendererShadows>(GraphicsStage(0, 0));
	
		Renderer::Get()->AddRenderer<RendererMeshes>(GraphicsStage(1, 0));
		Renderer::Get()->AddRenderer<RendererDeferred>(GraphicsStage(1, 1));
		Renderer::Get()->AddRenderer<RendererMeshes>(GraphicsStage(1, 1), SORT_BACK);
	//	Renderer::Get()->AddRenderer<RendererParticles>(GraphicsStage(1, 1));
		Renderer::Get()->AddRenderer<FilterDefault>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterFxaa>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterLensflare>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterTiltshift>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<FilterDamage>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterGrain>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<RendererGuis>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<RendererFonts>(GraphicsStage(1, 2));
	}

	void MainRenderer::Update()
	{
		auto &renderpassCreate0 = Renderer::Get()->GetRenderStage(0)->GetRenderpassCreate();
		renderpassCreate0.SetWidth(Shadows::Get()->GetShadowSize());
		renderpassCreate0.SetHeight(Shadows::Get()->GetShadowSize());

		/*auto filterDamage = GetRenderer<FilterDamage>();

		if (filterDamage != nullptr)
		{
			filterDamage->SetColour(Colour::RED);
			filterDamage->SetRadiusDriver<DriverConstant>(0.55f);
			filterDamage->SetSoftnessDriver<DriverSinwave>(0.25f, 0.35f, 2.0f);
		}*/
	}
}
