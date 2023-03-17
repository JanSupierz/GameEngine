#pragma once
#include "Component.h"
#include "imgui_plot.h"
#include <vector>

namespace dae
{
	class TrashTheCasheComponent final : public Component
	{
	public:
		virtual void Update() override;
		virtual void Render() override;

		TrashTheCasheComponent(int priority = -2);
		virtual ~TrashTheCasheComponent() = default;
		TrashTheCasheComponent(const TrashTheCasheComponent& other) = default;
		TrashTheCasheComponent(TrashTheCasheComponent&& other) = default;
		TrashTheCasheComponent& operator=(const TrashTheCasheComponent& other) = default;
		TrashTheCasheComponent& operator=(TrashTheCasheComponent&& other) = default;

	private:
		void UpdatePlotConfig();
		float CalculateAverage(std::vector<float>& testResults) const;
		void TestCashSpeed_01();

		ImGui::PlotConfig m_PlotConfig{};
		std::vector<float> m_XValues{};
		std::vector<float> m_YValues{};

		int m_NrTests{10};
		int m_NrTestValues{10000000};

		bool m_ShouldRecalculateValues{ true };	
	};
}

