#include "UiNavigation.hpp"

#include <Maths/Visual/DriverSlide.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include "ContentExit.hpp"

namespace micro
{
	const std::vector<std::pair<std::string, Colour>> TABS =
	{
		{"Play", Colour("#B8312F")},
		{"News", Colour("#2969af")},
		{"Achievements", Colour("#41A85F")},
		{"Settings", Colour("#475577")},
		{"Exit", Colour("#75706B")}
	};

	UiNavigation::UiNavigation(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_barBackground(nullptr),
		m_barTitle(nullptr),
		m_barCreatedBy(nullptr),
		m_tabPuck(nullptr),
		m_tabs(std::vector<std::unique_ptr<UiTab>>()),
		m_driverTarget(nullptr),
		m_currentTab(nullptr),
		m_targetTab(nullptr)
	{
		m_barBackground = std::make_unique<Gui>(this, UiBound(Vector2(0.5f, 1.0f), "TopCentre", true, false, Vector2(1.0f, 1.0f)), Texture::Resource("Guis/Gradient.png"));
		m_barBackground->SetScissor(Vector4(0.0f, 0.0f, 1.0f, 0.125f));
		m_barBackground->SetColourOffset(Colour("#2969B0"));

		m_barTitle = std::make_unique<Text>(this, UiBound(Vector2(0.01f, 0.985f), "LeftCentre", false), 3.5f, "Microverse", FontType::Resource("Fonts/ProximaNova", "Bold"), JUSTIFY_LEFT, 1.0f, 0.001f);
		m_barCreatedBy = std::make_unique<Text>(this, UiBound(Vector2(0.02f, 0.915f), "LeftCentre", false), 1.1f, "Created By: Equilibrium Games", FontType::Resource("Fonts/ProximaNova", "Light"), JUSTIFY_LEFT, 1.0f, 0.0013f);

		m_tabPuck = std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 0.875f), "BottomLeft", false, true, Vector2(0.0f, 0.01f)), Texture::Resource("Guis/White.png"));
		m_tabPuck->SetColourOffset(Colour("#386AB5"));

		float tabXOffset = 0.35f;
		uint32_t i = 0;

		for (auto &tabType : TABS)
		{
			UiBound rectangle = UiBound(Vector2(tabXOffset, 0.955f), "TopLeft", false);
			auto uiTab = new UiTab(this, new ContentExit(this), rectangle, tabType.first, tabType.second);
			tabXOffset += 0.03f + uiTab->GetWidth();
			m_tabs.emplace_back(uiTab);

			if (i == 0)
			{
				SwitchTab(uiTab->GetName());
			}

			i++;
		}
	}

	void UiNavigation::UpdateObject()
	{
		if (m_driverTarget != nullptr && m_targetTab != nullptr)
		{
			float progress = m_driverTarget->Update(Engine::Get()->GetDelta());

			if (m_currentTab != nullptr)
			{
				m_barBackground->SetColourOffset(m_currentTab->GetColour().Interpolate(m_targetTab->GetColour(), progress));
				m_tabPuck->GetRectangle().m_position.m_x = Maths::Interpolate(m_currentTab->GetRectangle().m_position.m_x, m_targetTab->GetRectangle().m_position.m_x, progress);
				m_tabPuck->GetRectangle().m_dimensions.m_x = Maths::Interpolate(m_currentTab->GetWidth(), m_targetTab->GetWidth(), progress);
			}
			else
			{
				progress = 1.0f;
				m_barBackground->SetColourOffset(m_targetTab->GetColour());
				m_tabPuck->GetRectangle().m_position.m_x = m_targetTab->GetRectangle().m_position.m_x;
				m_tabPuck->GetRectangle().m_dimensions.m_x = m_targetTab->GetWidth();
			}

			if (progress == 1.0f)
			{
				m_currentTab = m_targetTab;
				m_targetTab = nullptr;
			}
		}
	}

	void UiNavigation::SwitchTab(const std::string &tabName)
	{
		if (m_targetTab != nullptr)
		{
			return;
		}

		for (auto &tab : m_tabs)
		{
			if (tab->GetName() == tabName)
			{
				if (m_currentTab != nullptr && m_currentTab->GetName() == tabName)
				{
					continue;
				}

				m_driverTarget = std::make_unique<DriverSlide>(0.0f, 1.0f, 0.4f);
				m_targetTab = tab.get();

				if (m_currentTab != nullptr)
				{
					m_currentTab->GetContent()->SetAlphaDriver<DriverSlide>(1.0f, 0.0f, 0.1f);
				}

				Events::Get()->AddEvent<EventTime>(0.32f, [&]()
				{
					m_targetTab->GetContent()->SetAlphaDriver<DriverSlide>(0.0f, 1.0f, 0.1f);
				}, false);
			}
		}
	}
}
