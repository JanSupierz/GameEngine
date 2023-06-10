#include "HUDEvent.h"

dae::HUDEvent::HUDEvent(HUDEventType type, int index, int value)
    :m_Index{ index }, m_Value{ value }, m_Type{ type }
{
}

int dae::HUDEvent::GetIndex() const
{
    return m_Index;
}

int dae::HUDEvent::GetValue() const
{
    return m_Value;
}

dae::HUDEventType dae::HUDEvent::GetType() const
{
    return m_Type;
}

