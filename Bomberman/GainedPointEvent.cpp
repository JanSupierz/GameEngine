#include "GainedPointEvent.h"

dae::GainedPointEvent::GainedPointEvent(const std::string& name, int score)
    :m_Name{ name }, m_Score{ score }
{
}

std::string dae::GainedPointEvent::GetName() const
{
    return m_Name;
}

int dae::GainedPointEvent::GetScore() const
{
    return m_Score;
}

