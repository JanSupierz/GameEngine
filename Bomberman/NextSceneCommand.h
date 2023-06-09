#pragma once
#include "Command.h"

namespace dae
{
    class NextSceneCommand final : public Command
    {
    public:
        NextSceneCommand();
        void Execute() override;
    };
}

