#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>
#include <mutex>
#include <glm/glm.hpp>
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class Scene;
}
class TileComponent;
enum class Character;
struct MovementInfo;
struct Disk;

enum class TileType
{
	Tile,
	Disk,
	None
};

enum class GameState
{
	NextRound,
	NextLevel,
	QbertDied
};

struct CharacterInfo
{
	glm::ivec2 tileIndex{ -1 };
	bool isMoving{ false };
	glm::ivec2 previousTileIndex{ 0 };
};

// Comparator function for ivec2 to be used in map m_Tiles
struct ivec2_compare
{
	bool operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) const
	{
		if (lhs.x != rhs.x)
			return lhs.x < rhs.x;
		return lhs.y < rhs.y;
	}
};

class LevelManagerComponent final : public dae::BaseComponent, public dae::Observer<Character, MovementInfo>, public dae::Observer<Character, bool>, public dae::Observer<Character, dae::GameObject*>, public dae::Observer<Disk, Character>, public dae::Observer<GameState>
{
public:
	LevelManagerComponent(dae::GameObject* pGameObject);
	virtual ~LevelManagerComponent() override;

	LevelManagerComponent(const LevelManagerComponent& other) = delete;
	LevelManagerComponent(LevelManagerComponent&& other) noexcept = delete;
	LevelManagerComponent& operator=(const LevelManagerComponent& other) = delete;
	LevelManagerComponent& operator=(LevelManagerComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	// MoveStateChanged
	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject) override;

	// CharacterSpawned
	virtual void Notify(Character character, dae::GameObject* pGameObject) override;
	virtual void SubjectDestroyed(dae::Subject<Character, dae::GameObject*>* pSubject) override;

	// TileChanged
	virtual void Notify(Character character, bool roundFinished) override;

	// DiskStateChanged
	virtual void Notify(Disk disk, Character character) override;
	virtual void SubjectDestroyed(dae::Subject<Disk, Character>* pSubject) override;

	// GameResumed
	virtual void Notify(GameState gameState) override;
	
	void SkipRound();

	int GetRoundNr() const { return m_CurrentRound; }
	int GetLevelNr() const { return m_CurrentLevel; }
	bool IsGamePaused() const { return m_GamePaused; }
	glm::ivec2 GetTileSize() const { return m_TileSize; }

	glm::vec3 GetTilePos(glm::ivec2 tileIdx) const;
	glm::vec3 GetWorldTilePos(glm::ivec2 tileIdx) const;
	TileType GetTileType(Character character) const;
	int GetAmountOfActiveDisks() const;
	std::pair<Character, CharacterInfo> GetCharacter(Character character) const;
	const std::map<glm::ivec2, dae::GameObject*, ivec2_compare>& GetTiles() const { return m_Tiles; }

	std::unique_ptr<dae::Subject<Character, Character>> CharactersCollide;
	std::unique_ptr<dae::Subject<Character, bool>> TileChanged;
	std::unique_ptr<dae::Subject<GameState>> GameResumed;
private:
	glm::ivec2 GetNewDiskIndex() const;
	bool AreAllTilesCovered() const;
	void LandOnTile(Character character, TileComponent* pTileComponent);

	dae::Subject<Character, MovementInfo>* m_pMoveStateChangedSubject{ nullptr };
	dae::Subject<Character, dae::GameObject*>* m_pCharacterSpawnedSubject{ nullptr };
	dae::Subject<Disk, Character>* m_pDiskStateChanged{ nullptr };

	std::map<glm::ivec2, dae::GameObject*, ivec2_compare> m_Tiles;
	std::unordered_map<Character, CharacterInfo> m_Characters;

	std::vector<dae::GameObject*> m_vInactiveDisks;

	glm::ivec2 m_TileSize{ 0 };
	glm::ivec2 m_DiskSize{ 0 };
	const int m_LevelLength{ 7 };
	int m_TilesCovered{ 0 };
	int m_CurrentRound{ 1 };
	int m_CurrentLevel{ 1 };
	int m_AmountOfDisks{ 2 };
	bool m_CharacterMovedDirtyFlag{ false };

	bool m_GamePaused{ true };
	float m_AccumSec{};
	float m_PauseDuration{ 2.5f };
};

