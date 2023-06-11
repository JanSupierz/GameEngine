#pragma once
#include "Command.h"

namespace dae
{
    class ToggleMuteCommand final : public Command
    {
    public:
        ToggleMuteCommand();
        void Execute() override;
    };
}

