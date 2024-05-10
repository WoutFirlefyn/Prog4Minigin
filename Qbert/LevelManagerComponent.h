#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <vector>
#include <memory>
#include <unordered_map>
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// LevelManagerComponent Class									
//-----------------------------------------------------
namespace dae
{
	class Scene;
}
class TileComponent;
enum class Character;
enum class MovementState;
enum class MovementDirection;

enum class TileType
{
	Tile,
	Disk,
	None
};

class LevelManagerComponent final : public dae::BaseComponent, public dae::Observer<Character, MovementState, MovementDirection>, public dae::Observer<bool>, public dae::Observer<Character>
{
public:
	LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene);
	~LevelManagerComponent();

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	LevelManagerComponent(const LevelManagerComponent& other) = delete;
	LevelManagerComponent(LevelManagerComponent&& other) noexcept = delete;
	LevelManagerComponent& operator=(const LevelManagerComponent& other) = delete;
	LevelManagerComponent& operator=(LevelManagerComponent&& other)	noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void AddCharacters(const std::unordered_map<Character, dae::GameObject*>& characters) { m_InactiveCharacters = characters; }

	virtual void Init() override;
	virtual void LateUpdate() override;

	void AddObserver(dae::Subject<Character, MovementState, MovementDirection>* pMoveStateChanged);
	virtual void Notify(Character character, MovementState movementState, MovementDirection movementDirection) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementState, MovementDirection>* pSubject) override;

	virtual void Notify(Character character) override;
	virtual void SubjectDestroyed(dae::Subject<Character>* pSubject) override;

	virtual void Notify(bool roundFinished) override;

	static std::unique_ptr<dae::Subject<Character, TileType>> CharacterStartedJumping;
	std::unique_ptr<dae::Subject<bool>> TileChanged;
private:
	bool AreAllTilesCovered() const;
	dae::GameObject* FindCharacter(Character character) const;

	dae::Subject<Character, MovementState, MovementDirection>* m_pMoveStateChangedSubject{ nullptr };
	dae::Subject<Character>* m_pCharacterSpawnedSubject{ nullptr };
	std::unordered_map<Character, dae::GameObject*> m_InactiveCharacters;
	std::vector<dae::GameObject*> m_vTiles;
	const int m_LevelLength{ 7 };
	int m_TilesCovered{ 0 };
	int m_CurrentRound{ 0 };
	int m_AmountOfDisks{ 2 };
};

