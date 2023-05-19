#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
    class Value2DCommand : public Command
    {
    public:
        explicit Value2DCommand() = default;
        virtual ~Value2DCommand() = default;

        virtual void Execute() = 0;
        void SetValue(const glm::vec2& value);

    protected:
        glm::vec2 m_Value{};
    };
}

