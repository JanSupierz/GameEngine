#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
    class AxisValueCommand : public Command
    {
    public:
        explicit AxisValueCommand();
        virtual ~AxisValueCommand() = default;

        virtual void Execute() = 0;
        void SetValue(const glm::vec2& value);

    protected:
        glm::vec2 m_Value{};
    };
}

