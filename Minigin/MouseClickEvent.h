#pragma once
#include "QueueEvent.h"
#include <glm/glm.hpp>

namespace dae
{
    enum class MouseButton;

    class MouseClickEvent final : public QueueEvent
    {
    public:
        MouseClickEvent(const glm::vec2& mousePos, MouseButton button);
        glm::vec2 GetMousePos() const;
        MouseButton GetMouseButton() const;
    private:
        glm::vec2 m_MousePos;
        MouseButton m_Button;
    };
}

