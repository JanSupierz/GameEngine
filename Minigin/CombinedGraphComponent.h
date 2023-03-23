#pragma once
#include "GraphComponent.h"

namespace dae
{
	class CombinedGraphComponent final : public GraphComponent
	{
	public:
		virtual void Update() override;
		virtual void Render() override;

		CombinedGraphComponent(const std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)>& testFunction, 
			const std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)>& secondTestFunction, const std::string& title, int priority = -2);
		virtual ~CombinedGraphComponent() = default;
		CombinedGraphComponent(const CombinedGraphComponent& other) = default;
		CombinedGraphComponent(CombinedGraphComponent&& other) = default;
		CombinedGraphComponent& operator=(const CombinedGraphComponent& other) = default;
		CombinedGraphComponent& operator=(CombinedGraphComponent&& other) = default;
	private:
		void UpdatePlotConfig_02();
		void UpdatePlotConfig_03();

		std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)> f_SecondTestFunction;

		ImGui::PlotConfig m_PlotConfig_02{};
		ImGui::PlotConfig m_PlotConfig_03{};

		std::vector<float> m_XValues_02{};
		std::vector<float> m_YValues_02{};
		std::vector<const float*> m_ValueList{};

		bool m_ShouldRecalculateValues_02{ true };
	};
}

