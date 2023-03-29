#include "SingleValueCommand.h"

using namespace dae;

SingleValueCommand::SingleValueCommand()
    : Command{}
{

}

void dae::SingleValueCommand::SetValue(float value)
{
    m_Value = value;
}
