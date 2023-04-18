#pragma once
#include "Singleton.h"

#include <vector>
#include <string>
#include "Observer.h"

namespace dae
{
	#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
	struct Achievement_t
	{
		int m_eAchievementID{};
		const char* m_pchAchievementID{};
		char m_rgchName[128]{};
		char m_rgchDescription[256]{};
		bool m_bAchieved{};
		int m_iIconImage{};
	};

	//Define your achievements
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	class GainedPointEvent;

	class SteamAchievements final : public dae::Singleton<SteamAchievements>, public Observer<GainedPointEvent>
	{
	public:
		void Initialize(std::vector<Achievement_t>& achievemnets, bool reset);
		virtual void OnNotify(const GainedPointEvent& event) override;

	private:
		bool SetAchievement(EAchievements achievement);
		std::vector<Achievement_t> m_Achievements;
	};
}