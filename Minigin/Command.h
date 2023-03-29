#pragma once
namespace dae
{
    enum class ButtonState
    {
        Up, Down, Pressed
    };

    class Command {
    public:
        virtual ~Command() {}
        virtual void Execute() = 0;
    };
}
