#include "GraphComponent.h"
#include "imgui.h"

void dae::GraphComponent::Update()
{
    if (m_ShouldRecalculateValues)
    {
        f_TestFunction(m_XValues, m_YValues, m_NrTests, m_NrTestValues);
        UpdatePlotConfig();
    }
}

void dae::GraphComponent::Render()
{
    ImGui::Begin(m_GraphTitle.c_str());
    ImGui::SliderInt("Array Size", &m_NrTestValues, 1000000, 20000000);
    ImGui::SliderInt("Number of tests", &m_NrTests, 1, 100);

    ImGui::Spacing();

    if (ImGui::Button("Calculate Graph"))
    {
        m_ShouldRecalculateValues = true;
    }

    ImGui::Spacing();

    if (!m_ShouldRecalculateValues)
    {
        ImGui::Plot("Graph", m_PlotConfig);
    }
    else
    {
        ImGui::Text("Wait for the result...");
    }

    ImGui::End();
}

dae::GraphComponent::GraphComponent(const std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)>& testFunction, const std::string& title, int priority)
    :Component(priority), f_TestFunction{ testFunction }, m_GraphTitle{ title }
{
    ImU32 graphColor{ ImColor(255, 132, 69) };

    m_PlotConfig.values.color = graphColor;
    m_PlotConfig.tooltip.show = false;
    m_PlotConfig.tooltip.format = "x%.2f, y=%.2f";
    m_PlotConfig.grid_y.show = true;
    m_PlotConfig.grid_y.size = 5.f;
    m_PlotConfig.grid_y.subticks = 1;
    m_PlotConfig.frame_size = ImVec2(200, 75);
    m_PlotConfig.line_thickness = 1.f;
}

void dae::GraphComponent::UpdatePlotConfig()
{
    m_PlotConfig.values.xs = m_XValues.data();
    m_PlotConfig.values.ys = m_YValues.data();
    m_PlotConfig.values.count = static_cast<int>(m_XValues.size());
    m_PlotConfig.scale.min = 0;

    const auto maxElement{ std::max_element(m_YValues.begin(), m_YValues.end()) };
    if (maxElement == m_YValues.end())
    {
        m_PlotConfig.scale.max = 0;
    }
    else
    {
        m_PlotConfig.scale.max = *maxElement;
    }

    m_ShouldRecalculateValues = false;
}

