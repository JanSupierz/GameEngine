#pragma once
#include "Command.h"

namespace dae
{
    class GameObject;

    class PlaceBombCommand final : public Command
    {
    public:
        PlaceBombCommand(GameObject* pGameObject);
        void Execute() override;

    private:
        GameObject* m_pGameObject;
    };
}

