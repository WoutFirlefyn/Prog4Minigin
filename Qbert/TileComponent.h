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
class TileComponent final : public dae::BaseComponent, public dae::Observer<Character, bool>, public dae::Observer<bool>
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

	virtual void Notify(Character, bool roundFinished) override;
	virtual void SubjectDestroyed(dae::Subject<Character, bool>* pSubject) override;
	virtual void Notify(bool nextLevel) override;
	virtual void SubjectDestroyed(dae::Subject<bool>* pSubject) override;

	bool ChangeTile(int& tilesCovered, int stageChange);

	int GetMaxTileStage() { return m_MaxTileStage; }
private:
	dae::Subject<Character, bool>* m_pTileChangedSubject{ nullptr };
	dae::Subject<bool>* m_pNewRoundStartedSubject{ nullptr };
	dae::SpritesheetComponent* m_pSpritesheetComponent{ nullptr };
	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };

	int m_TileStage{ 0 };
	int m_MaxTileStage{ 1 };

	float m_AccumSec{};
	float m_Fps{ 12.f };
};


 
