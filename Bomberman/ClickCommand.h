#pragma once
#include "Value2DCommand.h"

namespace dae
{
    enum class MouseButton;

    class ClickCommand final : public Value2DCommand
    {
    public:
        ClickCommand(MouseButton button);
        void Execute() override;

    private:
        MouseButton m_Button;
    };
}

