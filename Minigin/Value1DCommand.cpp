#include "Value1DCommand.h"

using namespace dae;

Value1DCommand::Value1DCommand()
    : Command{}
{
}

void dae::Value1DCommand::SetValue(float value)
{
    m_Value = value;
}
