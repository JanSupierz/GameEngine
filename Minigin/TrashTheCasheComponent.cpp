#include "TrashTheCasheComponent.h"
#include "imgui.h"

#include <algorithm>
#include <numeric>
#include <chrono>

void dae::TrashTheCasheComponent::Update()
{
    if (m_ShouldRecalculateValues)
    {
        TestCashSpeed_01();
        UpdatePlotConfig();
    }
}

void dae::TrashTheCasheComponent::Render()
{
	ImGui::Begin("Exercise 1");

	ImGui::Value("hello", 5);
	ImGui::Plot("hello", m_PlotConfig);
	ImGui::End();
}

dae::TrashTheCasheComponent::TrashTheCasheComponent(int priority)
	:Component(priority)
{
    ImU32 graphColor{ ImColor(217, 132, 69) };

    m_PlotConfig.values.color = graphColor;
    m_PlotConfig.tooltip.show = false;
    m_PlotConfig.tooltip.format = "x%.2f, y=%.2f";
    m_PlotConfig.grid_y.show = true;
    m_PlotConfig.grid_y.size = 5.f;
    m_PlotConfig.grid_y.subticks = 1;
    m_PlotConfig.frame_size = ImVec2(150, 75);
    m_PlotConfig.line_thickness = 1.f;
}

void dae::TrashTheCasheComponent::UpdatePlotConfig()
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

float dae::TrashTheCasheComponent::CalculateAverage(std::vector<float>& testResults) const
{
    const int tenPercent{ static_cast<int>(testResults.size() * 0.1f) };

    std::sort(testResults.begin(), testResults.end());

    const float totalTime{ std::accumulate(testResults.begin() + tenPercent, testResults.end() - tenPercent, 0.f) };
    const float averageTime{ totalTime / (testResults.size() - 2 * tenPercent) };

    return averageTime;
}

void dae::TrashTheCasheComponent::TestCashSpeed_01()
{
    int* values{ new int[m_NrTestValues] {} };
    std::vector<float> testResults{};

    for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
    {
        for (int testIndex{ 0 }; testIndex < m_NrTests; ++testIndex)
        {
            const auto start{ std::chrono::high_resolution_clock::now() };
            for (int index{}; index < m_NrTestValues; index += stepsize)
            {
                values[index] *= 2;
            }
            const auto end{ std::chrono::high_resolution_clock::now() };
            const auto elapsedTime{ std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() };
            testResults.push_back(elapsedTime / 1000.f);
        }
        
        m_XValues.push_back(static_cast<float>(stepsize));
        m_YValues.push_back(CalculateAverage(testResults));
        
        testResults.clear();
    }
    delete[] values;
}
