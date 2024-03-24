//---------------------------
// Includes
//---------------------------
#include "AchievementComponent.h"
#include "Achievement.h"
#include "QbertComponent.h"

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }

//---------------------------
// Constructor & Destructor
//---------------------------
dae::AchievementComponent::AchievementComponent(GameObject* pGameObject, QbertComponent* pQbertComponent)
	: BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
{
	m_vAchievements.push_back( _ACH_ID(ACH_WIN_ONE_GAME, "Winner") );
	m_vAchievements.push_back( _ACH_ID(ACH_WIN_100_GAMES, "Champion"));
	m_vAchievements.push_back( _ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"));
	m_vAchievements.push_back( _ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"));

	m_pSteamAchievements = std::make_unique<CSteamAchievements>(m_vAchievements.data(), static_cast<int>(m_vAchievements.size()));
}

dae::AchievementComponent::~AchievementComponent()
{
	if (m_pQbertComponent)
		m_pQbertComponent->ScoreChanged->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void dae::AchievementComponent::Init()
{
	m_pQbertComponent->ScoreChanged->AddObserver(this);
}

void dae::AchievementComponent::Notify()
{
	int score = m_pQbertComponent->GetScore();

	if (score >= 500)
		UnlockAchievement(ACH_WIN_ONE_GAME);
	if (score >= 1000)
		UnlockAchievement(ACH_WIN_100_GAMES);
	if (score >= 1500)
		UnlockAchievement(ACH_TRAVEL_FAR_ACCUM);
	if (score >= 2000)
		UnlockAchievement(ACH_TRAVEL_FAR_SINGLE);
}

void dae::AchievementComponent::SubjectDestroyed()
{
	m_pQbertComponent = nullptr;
}

void dae::AchievementComponent::UnlockAchievement(EAchievements achievement)
{
	if (m_pSteamAchievements && !m_vAchievements[static_cast<int>(achievement)].m_bAchieved)
		m_pSteamAchievements->SetAchievement(m_vAchievements[static_cast<int>(achievement)].m_pchAchievementID);
}
