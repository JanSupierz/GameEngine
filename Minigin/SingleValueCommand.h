#pragma once
#include "Command.h"

namespace dae
{
    class SingleValueCommand : public Command
    {
    public:
        explicit SingleValueCommand();
        virtual ~SingleValueCommand() = default;

        virtual void Execute() = 0;
        void SetValue(float value);

    protected:
        float m_Value{};
    };
}

