#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class LevelManagerComponent;
enum class GameState;
class TileIconComponent final : public dae::BaseComponent, public dae::Observer<GameState>
{
public:
	TileIconComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~TileIconComponent() override;

	TileIconComponent(const TileIconComponent& other) = delete;
	TileIconComponent(TileIconComponent&& other) noexcept = delete;
	TileIconComponent& operator=(const TileIconComponent& other) = delete;
	TileIconComponent& operator=(TileIconComponent&& other)	noexcept = delete;

	virtual void Init() override;

	virtual void Notify(GameState gameState) override;
	virtual void SubjectDestroyed(dae::Subject<GameState>* pSubject) override;
private:
	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	dae::Subject<GameState>* m_pGameResumedSubject{ nullptr };
};

