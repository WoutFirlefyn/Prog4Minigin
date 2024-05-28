#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <vector>
#include <unordered_map>

namespace dae 
{
	class SpritesheetComponent;
}
enum class Character;
enum class MovementDirection;
class LevelManagerComponent;
class TileComponent final : public dae::BaseComponent, public dae::Observer<bool>, public dae::Observer<>
{
public:
	TileComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);	
	virtual ~TileComponent() override;	

	TileComponent(const TileComponent& other)					= delete;
	TileComponent(TileComponent&& other) noexcept				= delete;
	TileComponent& operator=(const TileComponent& other)		= delete;
	TileComponent& operator=(TileComponent&& other)	noexcept	= delete;

	virtual void Init() override;
	virtual void Update() override;

	virtual void Notify(bool roundFinished) override;
	virtual void SubjectDestroyed(dae::Subject<bool>* pSubject) override;
	virtual void Notify() override;
	virtual void SubjectDestroyed(dae::Subject<>* pSubject) override;

	bool ChangeTile(int& tilesCovered, int stageChange);

	static int GetMaxTileStage() { return m_MaxTileStage; }
private:
	dae::Subject<bool>* m_pTileChanged{ nullptr };
	dae::Subject<>* m_pNewRoundStarted{ nullptr };
	dae::SpritesheetComponent* m_pSpritesheetComponent{ nullptr };

	int m_TileStage{ 0 };
	static int m_MaxTileStage;

	float m_AccumSec{};
	float m_Fps{ 12.f };
};


 
