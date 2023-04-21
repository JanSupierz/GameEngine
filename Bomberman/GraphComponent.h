#pragma once
#include "Component.h"
#include "imgui_plot.h"
#include <vector>
#include <functional>
#include <string>

namespace dae
{
	class GraphComponent : public Component
	{
	public:
		virtual void Update() override;
		virtual void Render() override;

		GraphComponent(const std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)>& testFunction,const std::string& title, int priority = -2);
		virtual ~GraphComponent() = default;
		GraphComponent(const GraphComponent& other) = default;
		GraphComponent(GraphComponent&& other) = default;
		GraphComponent& operator=(const GraphComponent& other) = default;
		GraphComponent& operator=(GraphComponent&& other) = default;

	protected:
		void UpdatePlotConfig();

		std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)> f_TestFunction{};
		ImGui::PlotConfig m_PlotConfig{};
		std::vector<float> m_XValues{};
		std::vector<float> m_YValues{};
		std::string m_GraphTitle;
		int m_NrTests{ 10 };
		int m_NrTestValues{ 10000000 };

		bool m_ShouldRecalculateValues{ true };
	};
}

