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
		{
			Attachment(0, "shadows", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8_UNORM)
		}, // images
		{
			SubpassType(0, {0})
		}, // subpasses
		4096, 4096 // width, height
	};
	const RenderpassCreate RENDERPASS_1_CREATE = RenderpassCreate
	{
		{
			Attachment(0, "depth", ATTACHMENT_TYPE_DEPTH, VK_FORMAT_D32_SFLOAT_S8_UINT, false),
			Attachment(1, "swapchain", ATTACHMENT_TYPE_SWAPCHAIN),
			Attachment(2, "diffuse", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false),
			Attachment(3, "normals", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R16G16B16A16_SFLOAT, false),
			Attachment(4, "materials", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM, false),
			Attachment(5, "resolved", ATTACHMENT_TYPE_IMAGE, VK_FORMAT_R8G8B8A8_UNORM)
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

		Renderer::Get()->AddRenderer<RendererDeferred>(GraphicsStage(1, 1), DEFERRED_SIMPLE);
		Renderer::Get()->AddRenderer<RendererMeshes>(GraphicsStage(1, 1), MESH_SORT_BACK);
	//	Renderer::Get()->AddRenderer<RendererParticles>(GraphicsStage(1, 1));

	//	Renderer::Get()->AddRenderer<FilterFxaa>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterLensflare>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterTiltshift>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<FilterDamage>(GraphicsStage(1, 2));
	//	Renderer::Get()->AddRenderer<FilterGrain>(GraphicsStage(1, 2));
		Renderer::Get()->AddRenderer<FilterDefault>(GraphicsStage(1, 2), true);
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
