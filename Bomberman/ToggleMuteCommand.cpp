#include "ToggleMuteCommand.h"
#include "Audio.h"

dae::ToggleMuteCommand::ToggleMuteCommand()
{
}

void dae::ToggleMuteCommand::Execute()
{
	Audio::Get().ToggleMute();
}
