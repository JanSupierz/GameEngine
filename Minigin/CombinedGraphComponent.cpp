#include "CombinedGraphComponent.h"
#include "imgui.h"

void dae::CombinedGraphComponent::Update()
{
    if (m_ShouldRecalculateValues)
    {
        f_TestFunction(m_XValues, m_YValues, m_NrTests, m_NrTestValues);
        UpdatePlotConfig();
        UpdatePlotConfig_03();
    }

    if (m_ShouldRecalculateValues_02)
    {
        f_SecondTestFunction(m_XValues_02, m_YValues_02, m_NrTests, m_NrTestValues);
        UpdatePlotConfig_02();
        UpdatePlotConfig_03();
    }
}

void dae::CombinedGraphComponent::Render()
{
    ImGui::Begin(m_GraphTitle.c_str());
    ImGui::SliderInt("Array Size", &m_NrTestValues, 1000000, 20000000);
    ImGui::SliderInt("Number of tests", &m_NrTests, 1, 100);

    ImGui::Spacing();

    if (ImGui::Button("Calculate First Graph"))
    {
        m_ShouldRecalculateValues = true;
    }

    ImGui::Spacing();

    if (!m_ShouldRecalculateValues)
    {
        ImGui::Plot("Graph_01", m_PlotConfig);
    }
    else
    {
        ImGui::Text("Wait for the result...");
    }

    if (ImGui::Button("Calculate Second Graph"))
    {
        m_ShouldRecalculateValues_02 = true;
    }

    ImGui::Spacing();

    if (!m_ShouldRecalculateValues_02)
    {
        ImGui::Plot("Graph_02", m_PlotConfig_02);
    }
    else
    {
        ImGui::Text("Wait for the result...");
    }

    ImGui::Spacing();

    m_PlotConfig_03.values.count = static_cast<int>(std::min(m_XValues.size(), m_XValues_02.size()));
    m_PlotConfig_03.scale.max = std::max(m_PlotConfig.scale.max, m_PlotConfig_02.scale.max);

    m_ValueList[0] = m_YValues.data();
    m_ValueList[1] = m_YValues_02.data();

    m_PlotConfig_03.values.ys_list = m_ValueList.data();

    ImGui::Plot("Graph_03", m_PlotConfig_03);

    ImGui::End();
}

dae::CombinedGraphComponent::CombinedGraphComponent(const std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)>& testFunction,
    const std::function<void(std::vector<float>& xValues, std::vector<float>& yValues, int nrTests, int nrTestValues)>& secondTestFunction, const std::string& title, int priority)
    :GraphComponent(testFunction, title, priority), f_SecondTestFunction{ secondTestFunction }
{
    //Init the 2nd graph
    ImU32 graphColor{ ImColor(50, 132, 129) };

    m_PlotConfig_02.values.color = graphColor;
    m_PlotConfig_02.tooltip.show = false;
    m_PlotConfig_02.tooltip.format = "x%.2f, y=%.2f";
    m_PlotConfig_02.grid_y.show = true;
    m_PlotConfig_02.grid_y.size = 5.f;
    m_PlotConfig_02.grid_y.subticks = 1;
    m_PlotConfig_02.frame_size = ImVec2(200, 75);
    m_PlotConfig_02.line_thickness = 1.f;

    //Init the combined graph
    static ImU32 colors[2] = { m_PlotConfig.values.color, m_PlotConfig_02.values.color };
    m_PlotConfig_03.values.colors = colors;

    m_PlotConfig_03.tooltip.show = false;
    m_PlotConfig_03.tooltip.format = "x%.2f, y=%.2f";
    m_PlotConfig_03.grid_y.show = true;
    m_PlotConfig_03.grid_y.size = 5.f;
    m_PlotConfig_03.grid_y.subticks = 1;
    m_PlotConfig_03.frame_size = ImVec2(200, 75);
    m_PlotConfig_03.line_thickness = 1.f;
    m_PlotConfig_03.values.ys_count = 2;

    m_ValueList.push_back(m_XValues.data());
    m_ValueList.push_back(m_XValues_02.data());
}

void dae::CombinedGraphComponent::UpdatePlotConfig_02()
{
    m_PlotConfig_02.values.xs = m_XValues_02.data();
    m_PlotConfig_02.values.ys = m_YValues_02.data();
    m_PlotConfig_02.values.count = static_cast<int>(m_XValues_02.size());
    m_PlotConfig_02.scale.min = 0;

    const auto maxElement{ std::max_element(m_YValues_02.begin(), m_YValues_02.end()) };
    if (maxElement == m_YValues_02.end())
    {
        m_PlotConfig_02.scale.max = 0;
    }
    else
    {
        m_PlotConfig_02.scale.max = *maxElement;
        m_PlotConfig_03.scale.max = *maxElement;
    }

    m_ShouldRecalculateValues_02 = false;
}

void dae::CombinedGraphComponent::UpdatePlotConfig_03()
{
    m_PlotConfig_03.values.count = static_cast<int>(std::min(m_XValues.size(), m_XValues_02.size()));
    m_PlotConfig_03.scale.max = std::max(m_PlotConfig.scale.max, m_PlotConfig_02.scale.max);
}

