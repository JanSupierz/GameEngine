#pragma once
#include "Command.h"

namespace dae
{
    class Value1DCommand : public Command
    {
    public:
        explicit Value1DCommand();
        virtual ~Value1DCommand() = default;

        virtual void Execute() = 0;
        void SetValue(float value);

    protected:
        float m_Value{};
    };
}

