#pragma once
#include <BaseComponent.h>
#include <Observer.h>
class LevelManagerComponent;
enum class GameState;
class TitleComponent final : public dae::BaseComponent, public dae::Observer<GameState>
{
public:
	TitleComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~TitleComponent() override = default;

	TitleComponent(const TitleComponent& other) = delete;
	TitleComponent(TitleComponent&& other) noexcept = delete;
	TitleComponent& operator=(const TitleComponent& other) = delete;
	TitleComponent& operator=(TitleComponent&& other) noexcept = delete;

	virtual void Init() override;

	virtual void Notify(GameState gameState) override;
	virtual void SubjectDestroyed(dae::Subject<GameState>* pSubject) override;
private:
	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	dae::Subject<GameState>* m_pGameResumedSubject{ nullptr };
};

