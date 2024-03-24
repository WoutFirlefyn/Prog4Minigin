#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"
#include "AchievementData.h"
#include "Achievement.h"
#include <vector>
#include <memory>

//-----------------------------------------------------
// AchievementComponent Class									
//-----------------------------------------------------
namespace dae
{
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	class QbertComponent;
	class CSteamAchievements;
	class AchievementComponent final : public BaseComponent, public Observer<>
	{
	public:
		AchievementComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);			// Constructor
		virtual ~AchievementComponent() override;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		AchievementComponent(const AchievementComponent& other) = delete;
		AchievementComponent(AchievementComponent&& other) noexcept = delete;
		AchievementComponent& operator=(const AchievementComponent& other) = delete;
		AchievementComponent& operator=(AchievementComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Notify() override;
		virtual void SubjectDestroyed() override;

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		void UnlockAchievement(EAchievements achievement);

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		std::vector<Achievement_t> m_vAchievements{};
		std::unique_ptr<CSteamAchievements> m_pSteamAchievements;
		QbertComponent* m_pQbertComponent;
	};
}

 
