#include "Value2DCommand.h"

using namespace dae;

Value2DCommand::Value2DCommand()
    : Command{}
{
}

void dae::Value2DCommand::SetValue(const glm::vec2& value)
{
    m_Value = value;
}

