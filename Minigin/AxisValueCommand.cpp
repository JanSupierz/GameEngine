#include "AxisValueCommand.h"

using namespace dae;

AxisValueCommand::AxisValueCommand()
    : Command{}
{

}

void dae::AxisValueCommand::SetValue(const glm::vec2& value)
{
    m_Value = value;
}

