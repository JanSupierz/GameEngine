#include "SteamAchievements.h"
#include "isteamuserstats.h"
#include "GainedPointEvent.h"
#include "PlayerComponent.h"

bool dae::SteamAchievements::SetAchievement(EAchievements achievement)
{
	auto it{ std::find_if(m_Achievements.begin(), m_Achievements.end(), [&](Achievement_t ach) {return ach.m_eAchievementID == achievement; }) };

	if (it != m_Achievements.end())
	{
		SteamUserStats()->SetAchievement(it->m_pchAchievementID);
		return SteamUserStats()->StoreStats();
	}
	else
	{
		return false;
	}
}

void dae::SteamAchievements::Initialize(std::vector<Achievement_t>& achievements, bool reset)
{
	m_Achievements = achievements;

	if (reset)
	{
		for (const auto& achievement : m_Achievements)
		{
			SteamUserStats()->ClearAchievement(achievement.m_pchAchievementID);
		}
	}
}

void dae::SteamAchievements::OnNotify(const GainedPointEvent& event)
{
	int score{ event.GetPlayer()->GetScore() };

	if (score >= 500)
	{
		SetAchievement(ACH_WIN_ONE_GAME);
	}
}