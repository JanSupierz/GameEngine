#include "AchievementsManager.h"
#include "SteamAchievements.h"

void dae::AchievementsManager::OnNotify(const void* pData, const std::string& event)
{
	if (event == "ScoreChanged")
	{
		int score{ *static_cast<const int*>(pData) };

		if (score >= 500)
		{
			SteamAchievements::GetInstance().SetAchievement(ACH_WIN_ONE_GAME);
		}
	}
}
