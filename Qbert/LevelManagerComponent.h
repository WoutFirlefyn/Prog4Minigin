#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>
#include <mutex>
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
struct MovementInfo;

enum class TileType
{
	Tile,
	Disk,
	None
};

class LevelManagerComponent final : public dae::BaseComponent, public dae::Observer<Character, MovementInfo>, public dae::Observer<bool>, public dae::Observer<Character>, public dae::Observer<dae::GameObject*, Character>
{
public:
	LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene);
	virtual ~LevelManagerComponent() override;

	LevelManagerComponent(const LevelManagerComponent& other) = delete;
	LevelManagerComponent(LevelManagerComponent&& other) noexcept = delete;
	LevelManagerComponent& operator=(const LevelManagerComponent& other) = delete;
	LevelManagerComponent& operator=(LevelManagerComponent&& other)	noexcept = delete;

	void AddCharacters(const std::unordered_map<Character, dae::GameObject*>& characters) { m_InactiveCharacters = characters; }

	virtual void Init() override;
	virtual void LateUpdate() override;

	// MoveStateChanged
	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject) override;

	// CharacterSpawned
	virtual void Notify(Character character) override;
	virtual void SubjectDestroyed(dae::Subject<Character>* pSubject) override;

	// TileChanged
	virtual void Notify(bool roundFinished) override;

	// DiskReachedTop
	virtual void Notify(dae::GameObject* pDisk, Character character) override;
	virtual void SubjectDestroyed(dae::Subject<dae::GameObject*, Character>* pSubject) override;

	static int GetRoundNr() { return m_CurrentRound; }

	static std::unique_ptr<dae::Subject<Character, TileType>> CharacterStartedJumping;
	static std::unique_ptr<dae::Subject<Character, Character>> CharactersCollide;
	std::unique_ptr<dae::Subject<bool>> TileChanged;
private:

	bool AreAllTilesCovered() const;
	void ChangeTile(Character character, TileComponent* pTileComponent);
	bool FindCharacter(Character character, std::pair<std::pair<int, int>, dae::GameObject*>& tile) const;

	dae::Subject<Character, MovementInfo>* m_pMoveStateChangedSubject{ nullptr };
	dae::Subject<Character>* m_pCharacterSpawnedSubject{ nullptr };
	dae::Subject<dae::GameObject*, Character>* m_pDiskReachedTopSubject{ nullptr };

	std::unordered_map<Character, dae::GameObject*> m_InactiveCharacters;
	std::unordered_map<Character, bool> m_MovingCharacters;

	std::map<std::pair<int, int>, dae::GameObject*> m_Tiles;

	std::vector<dae::GameObject*> m_vInactiveDisks;

	std::mutex m_CharactersCollideMutex{};
	const int m_LevelLength{ 7 };
	int m_TilesCovered{ 0 };
	static int m_CurrentRound;
	int m_AmountOfDisks{ 2 };
	bool m_CharacterMovedDirtyFlag{ false };
};

