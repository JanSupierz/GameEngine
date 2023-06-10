#pragma once
#include "Command.h"

namespace dae
{
    class PlayerComponent;

    class DetonateBombCommand final : public Command
    {
    public:
        DetonateBombCommand(PlayerComponent* pPlayer);
        void Execute() override;

    private:
        PlayerComponent* m_pPlayer;
    };
}

