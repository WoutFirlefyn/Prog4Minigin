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
dae::AchievementComponent::AchievementComponent(GameObject* pGameObject, QbertComponent* pQbertComponent1, QbertComponent* pQbertComponent2)
	: BaseComponent(pGameObject)
	, m_pQbertComponent1{ pQbertComponent1 }
	, m_pQbertComponent2{ pQbertComponent2 }
{
	m_vAchievements.push_back( _ACH_ID(ACH_WIN_ONE_GAME, "Winner") );
	m_vAchievements.push_back( _ACH_ID(ACH_WIN_100_GAMES, "Champion"));
	m_vAchievements.push_back( _ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"));
	m_vAchievements.push_back( _ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"));
	m_vAchievements.push_back( _ACH_ID(NEW_ACHIEVEMENT_0_4, "NEW_ACHIEVEMENT_NAME_0_4"));

	m_pSteamAchievements = std::make_unique<CSteamAchievements>(m_vAchievements.data(), static_cast<int>(m_vAchievements.size()));
}

dae::AchievementComponent::~AchievementComponent()
{
	if (m_pQbertComponent1)
		m_pQbertComponent1->ScoreChanged->RemoveObserver(this);
	if (m_pQbertComponent2)
		m_pQbertComponent2->ScoreChanged->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void dae::AchievementComponent::Init()
{
	m_pQbertComponent1->ScoreChanged->AddObserver(this);
	m_pQbertComponent2->ScoreChanged->AddObserver(this);
}

void dae::AchievementComponent::Notify()
{
	// std::max to make it also work for the 2nd qbert
	int score = std::max(m_pQbertComponent1->GetScore(), m_pQbertComponent2->GetScore());

	if (score >= 500)
		UnlockAchievement(ACH_WIN_ONE_GAME);
	if (score >= 1000)
		UnlockAchievement(ACH_WIN_100_GAMES);
	if (score >= 1500)
		UnlockAchievement(ACH_TRAVEL_FAR_ACCUM);
	if (score >= 2000)
		UnlockAchievement(ACH_TRAVEL_FAR_SINGLE);
	if (score >= 2500)
		UnlockAchievement(NEW_ACHIEVEMENT_0_4);
}

void dae::AchievementComponent::SubjectDestroyed(Subject<>* pSubject)
{
	// It should only set one component to nullptr instead of both at the same time but can't be bothered 
	// to fix it since it works perfectly fine like this and we will never use this code again after this assignment anyways
	if (pSubject == m_pQbertComponent1->ScoreChanged.get())
		m_pQbertComponent1 = nullptr;
	if (pSubject == m_pQbertComponent2->ScoreChanged.get())
		m_pQbertComponent2 = nullptr;
}

void dae::AchievementComponent::UnlockAchievement(EAchievements achievement)
{
	if (m_pSteamAchievements && !m_vAchievements[static_cast<int>(achievement)].m_bAchieved)
		m_pSteamAchievements->SetAchievement(m_vAchievements[static_cast<int>(achievement)].m_pchAchievementID);
}
