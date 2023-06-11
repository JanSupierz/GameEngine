#include "ClickCommand.h"
#include "EventManager.h"
#include "MouseClickEvent.h"
#include "Mouse.h"

dae::ClickCommand::ClickCommand(MouseButton button)
	:m_Button{ button }
{
}

void dae::ClickCommand::Execute()
{
	EventManager::GetInstance().AddEvent(std::make_shared<MouseClickEvent>(m_Value, m_Button));
}
