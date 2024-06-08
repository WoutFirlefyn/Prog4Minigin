#include "LivesComponent.h"
#include "GraphicsComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "Game.h"

LivesComponent::LivesComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pGameResumedSubject{ pLevelManagerComponent->GameResumed.get() }
{
	glm::ivec2 heartSize{ 0 };
	auto& scene = dae::SceneManager::GetInstance().GetCurrentScene();
	for (int i{}; i < m_Lives; ++i)
	{
		auto pHeart = std::make_unique<dae::GameObject>();
		pHeart->AddComponent<dae::GraphicsComponent>("Heart.png");
		pHeart->SetParent(GetGameObject());
		if (heartSize == glm::ivec2{ 0 })
			heartSize = pHeart->GetComponent<dae::GraphicsComponent>()->GetTextureSize();

		pHeart->SetPosition(0.f, static_cast<float>(heartSize.y * i));
		m_vHearts.push_back(scene.Add(std::move(pHeart)));
	}
}

LivesComponent::~LivesComponent()
{
	if (m_pPlayerDiedSubject)
		m_pPlayerDiedSubject->RemoveObserver(this);
}

void LivesComponent::Init()
{
	m_pPlayerDiedSubject->AddObserver(this);	
	m_pGameResumedSubject->AddObserver(this);
}

void LivesComponent::AddObserver(dae::Subject<>* pPlayerDiedSubject)
{
	m_pPlayerDiedSubject = pPlayerDiedSubject;
}

void LivesComponent::Notify()
{
	if (m_Lives-- > 0)
		m_vHearts[m_Lives]->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
}

void LivesComponent::SubjectDestroyed(dae::Subject<>* pSubject)
{
	if (pSubject == m_pPlayerDiedSubject)
		m_pPlayerDiedSubject = nullptr;
}

void LivesComponent::Notify(GameState gameState)
{
	if (gameState == GameState::QbertDied && m_Lives < 0)
		Game::GetInstance().SetScene(SceneType::GameOver);
}

void LivesComponent::SubjectDestroyed(dae::Subject<GameState>* pSubject)
{
	if (pSubject == m_pGameResumedSubject)
		m_pGameResumedSubject = nullptr;
}
