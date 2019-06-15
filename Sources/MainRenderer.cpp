#include "MainRenderer.hpp"

#include <Scenes/Scenes.hpp>
#include <Graphics/Graphics.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSinwave.hpp>
#include <Meshes/SubrenderMeshes.hpp>
#include <Fonts/SubrenderFonts.hpp>
#include <Guis/SubrenderGuis.hpp>
#include <Particles/SubrenderParticles.hpp>
#include <Shadows/SubrenderShadows.hpp>
#include <Post/Deferred/SubrenderDeferred.hpp>
#include <Post/Filters/FilterDefault.hpp>
#include <Post/Filters/FilterFxaa.hpp>
#include <Post/Filters/FilterGrain.hpp>
#include <Post/Filters/FilterLensflare.hpp>
#include <Post/Filters/FilterTiltshift.hpp>
#include "Post/Filters/FilterDamage.hpp"

namespace micro
{
	MainRenderer::MainRenderer()
	{
		std::vector<std::unique_ptr<RenderStage>> renderStages;
	
		std::vector<Attachment> renderpassAttachments0{ 
			{0, "shadows", Attachment::Type::Image, false, VK_FORMAT_R8_UNORM}
		};
		std::vector<SubpassType> renderpassSubpasses0{ 
			{0, {0}}
		};
		renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments0, renderpassSubpasses0, Viewport{{4096, 4096}}));

		std::vector<Attachment> renderpassAttachments1 { 
			{0, "depth", Attachment::Type::Depth, false},
			{1, "swapchain", Attachment::Type::Swapchain},
			{2, "position", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT},
			{3, "diffuse", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM},
			{4, "normal", Attachment::Type::Image, false, VK_FORMAT_R16G16B16A16_SFLOAT},
			{5, "material", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM},
			{6, "resolved", Attachment::Type::Image, false, VK_FORMAT_R8G8B8A8_UNORM}
		};
		std::vector<SubpassType> renderpassSubpasses1{ 
			{0, {0, 2, 3, 4, 5}},
			{1, {0, 6}},
			{2, {0, 1}}
		};
		renderStages.emplace_back(std::make_unique<RenderStage>(renderpassAttachments1, renderpassSubpasses1));
		Graphics::Get()->SetRenderStages(std::move(renderStages));

		Graphics::Get()->ClearSubrenders();

		//Graphics::Get()->AddSubrender<SubrenderShadows>({0, 0});

		Graphics::Get()->AddSubrender<SubrenderMeshes>({1, 0});

		Graphics::Get()->AddSubrender<SubrenderDeferred>({1, 1});
		Graphics::Get()->AddSubrender<SubrenderMeshes>({1, 1}, SubrenderMeshes::Sort::Back);
		//Graphics::Get()->AddSubrender<SubrenderParticles>({1, 1});

		//Graphics::Get()->AddSubrender<FilterFxaa>({1, 2});
		//Graphics::Get()->AddSubrender<FilterLensflare>({1, 2});
		//Graphics::Get()->AddSubrender<FilterTiltshift>({1, 2});
		Graphics::Get()->AddSubrender<FilterDamage>({1, 2});
		//Graphics::Get()->AddSubrender<FilterGrain>({1, 2});
		Graphics::Get()->AddSubrender<FilterDefault>({1, 2}, true);
		//Graphics::Get()->AddSubrender<RendererGizmos>({1, 2});
		Graphics::Get()->AddSubrender<SubrenderGuis>({1, 2});
		Graphics::Get()->AddSubrender<SubrenderFonts>({1, 2});
	}

	void MainRenderer::Update()
	{
		//auto renderpassCreate0 = Renderer::Get()->GetRenderStage(0);
		//renderpassCreate0->GetViewport().SetSize(Shadows::Get()->GetShadowSize());

		/*if (auto filterDamage{Graphics::Get()->GetSubrender<FilterDamage>()}; filterDamage != nullptr)
		{
			filterDamage->SetColour(Colour::Red);
			filterDamage->SetRadiusDriver<DriverConstant<float>>(0.55f);
			filterDamage->SetSoftnessDriver<DriverSinwave<float>>(0.25f, 0.35f, 2.0f);
		}*/
	}
}
