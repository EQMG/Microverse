#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Renderer/Renderer.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSinwave.hpp>

namespace test
{
	RenderpassCreate *RENDERPASS_0_CREATE = new RenderpassCreate
		{
			4096, 4096, // width / height
			{
				Attachment(0, ATTACHMENT_IMAGE, FORMAT_R8_UNORM) // shadows
			}, // images
			{
				SubpassType(0, {0})
			} // subpasses
		};
	RenderpassCreate *RENDERPASS_1_CREATE = new RenderpassCreate
		{
			0, 0, // width / height
			{
				Attachment(0, ATTACHMENT_DEPTH), // depth
				Attachment(1, ATTACHMENT_SWAPCHAIN), // swapchain
				Attachment(2, ATTACHMENT_IMAGE, FORMAT_R8G8B8A8_UNORM), // colours
				Attachment(3, ATTACHMENT_IMAGE, FORMAT_R16G16_UNORM), // normals
				Attachment(4, ATTACHMENT_IMAGE, FORMAT_R8G8B8A8_UNORM) // materials
			}, // images
			{
				SubpassType(0, {0, 2, 3, 4}),
				SubpassType(1, {1}),
				SubpassType(2, {1}),
			} // subpasses
		};

	MainRenderer::MainRenderer() :
		IManagerRender({RENDERPASS_0_CREATE, RENDERPASS_1_CREATE}),
		m_infinity(Vector4(0.0f, 1.0f, 0.0f, +INFINITY)),
		m_rendererShadows(RendererShadows({0, 0})),
		m_rendererMeshes(RendererMeshes({1, 0})),
		//	m_rendererParticles(RendererParticles({1, 0})),
		m_rendererDeferred(RendererDeferred({1, 1})),
		m_filterFxaa(FilterFxaa({1, 2})),
		m_filterLensflare(FilterLensflare({1, 2})),
		m_filterTiltshift(FilterTiltshift({1, 2})),
		m_filterDamage(FilterDamage({1, 2})),
		m_filterGrain(FilterGrain({1, 2})),
		m_rendererGuis(RendererGuis({1, 2})),
		m_rendererFonts(RendererFonts({1, 2}))
	{
//		m_filterLensflare.SetSunPosition(*Worlds::Get()->GetSunPosition());
//		m_filterLensflare.SetSunHeight(Worlds::Get()->GetSunHeight());

//		m_filterDamage.SetColour(Colour::RED);
//		m_filterDamage.SetRadiusDriver(new DriverConstant(0.55f));
//		m_filterDamage.SetSoftnessDriver(new DriverSinwave(0.25f, 0.35f, 2.0f));
	}

	MainRenderer::~MainRenderer()
	{
	}

	void MainRenderer::Render()
	{
		RenderPass0();
		RenderPass1();
	}

	void MainRenderer::RenderPass0()
	{
		RENDERPASS_0_CREATE->SetWidth(Shadows::Get()->GetShadowSize());
		RENDERPASS_0_CREATE->SetHeight(Shadows::Get()->GetShadowSize());

		auto commandBuffer = Renderer::Get()->GetCommandBuffer();
		auto camera = Scenes::Get()->GetCamera();

		// Starts Rendering.
		auto startResult = Renderer::Get()->StartRenderpass(*commandBuffer, 0);

		if (!startResult)
		{
			return;
		}

		// Subpass 0.
		m_rendererShadows.Render(*commandBuffer, m_infinity, *camera);

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(*commandBuffer, 0);
	}

	void MainRenderer::RenderPass1()
	{
		auto commandBuffer = Renderer::Get()->GetCommandBuffer();
		auto camera = Scenes::Get()->GetCamera();

		// Starts Rendering.
		auto startResult = Renderer::Get()->StartRenderpass(*commandBuffer, 1);

		if (!startResult)
		{
			return;
		}

		// Subpass 0.
		m_rendererMeshes.Render(*commandBuffer, m_infinity, *camera);
		//	m_rendererParticles.Render(*commandBuffer, m_infinity, *camera);
		Renderer::Get()->NextSubpass(*commandBuffer);

		// Subpass 1.
		m_rendererDeferred.Render(*commandBuffer, m_infinity, *camera);
		Renderer::Get()->NextSubpass(*commandBuffer);

		// Subpass 2.
#ifndef FL_BUILD_MACOS
		m_filterFxaa.Render(*commandBuffer, m_infinity, *camera);
		m_filterDamage.Render(*commandBuffer, m_infinity, *camera);
//		m_filterLensflare.Render(*commandBuffer, m_infinity, *camera);
//		m_filterTiltshift.Render(*commandBuffer, m_infinity, *camera);
//		m_filterGrain.Render(*commandBuffer, m_infinity, *camera);
#endif
		m_rendererGuis.Render(*commandBuffer, m_infinity, *camera);
		m_rendererFonts.Render(*commandBuffer, m_infinity, *camera);

		// Ends Rendering.
		Renderer::Get()->EndRenderpass(*commandBuffer, 1);
	}
}
