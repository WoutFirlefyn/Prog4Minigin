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

struct CharacterInfo
{
	glm::ivec2 tileIndex{-1};
	bool isMoving{ false };
};

class LevelManagerComponent final : public dae::BaseComponent, public dae::Observer<Character, MovementInfo>, public dae::Observer<bool>, public dae::Observer<Character, dae::GameObject*>, public dae::Observer<Disk, Character>
{
public:
	LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene);
	virtual ~LevelManagerComponent() override;

	LevelManagerComponent(const LevelManagerComponent& other) = delete;
	LevelManagerComponent(LevelManagerComponent&& other) noexcept = delete;
	LevelManagerComponent& operator=(const LevelManagerComponent& other) = delete;
	LevelManagerComponent& operator=(LevelManagerComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void LateUpdate() override;

	// MoveStateChanged
	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject) override;

	// CharacterSpawned
	virtual void Notify(Character character, dae::GameObject* pGameObject) override;
	virtual void SubjectDestroyed(dae::Subject<Character, dae::GameObject*>* pSubject) override;

	// TileChanged
	virtual void Notify(bool roundFinished) override;

	// DiskStateChanged
	virtual void Notify(Disk disk, Character character) override;
	virtual void SubjectDestroyed(dae::Subject<Disk, Character>* pSubject) override;

	static int GetRoundNr() { return m_CurrentRound; }

	TileType GetNextTileType(Character character, MovementInfo movementInfo) const;
	MovementInfo GetDirectionToNearestQbert() const;

	static std::unique_ptr<dae::Subject<Character, Character>> CharactersCollide;
	static std::unique_ptr<dae::Subject<bool>> TileChanged;
private:
	bool AreAllTilesCovered() const;
	void LandOnTile(Character character, TileComponent* pTileComponent);
	glm::vec3 GetTilePos(glm::ivec2 tileIdx) const;
	int CalculateManhattanDistance(const glm::ivec2& deltaPos) const;

	dae::Subject<Character, MovementInfo>* m_pMoveStateChangedSubject{ nullptr };
	dae::Subject<Character, dae::GameObject*>* m_pCharacterSpawnedSubject{ nullptr };
	dae::Subject<Disk, Character>* m_pDiskStateChanged{ nullptr };

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
	std::map<glm::ivec2, dae::GameObject*, ivec2_compare> m_Tiles;
	std::unordered_map<Character, CharacterInfo> m_Characters;

	std::vector<dae::GameObject*> m_vInactiveDisks;

	glm::ivec2 m_TileSize{};
	const int m_LevelLength{ 7 };
	int m_TilesCovered{ 0 };
	static int m_CurrentRound;
	int m_AmountOfDisks{ 2 };
	bool m_CharacterMovedDirtyFlag{ false };
};

