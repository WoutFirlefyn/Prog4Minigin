#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"
#include <vector>
#include <unordered_map>

//-----------------------------------------------------
// TileComponent Class									
//-----------------------------------------------------
enum class Character;
enum class MovementDirection;
class TileComponent final : public dae::BaseComponent
{
public:
	TileComponent(dae::GameObject* pGameObject);		// Constructor
	virtual ~TileComponent() override = default;		// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	TileComponent(const TileComponent& other)					= delete;
	TileComponent(TileComponent&& other) noexcept				= delete;
	TileComponent& operator=(const TileComponent& other)		= delete;
	TileComponent& operator=(TileComponent&& other)	noexcept	= delete;

	//-------------------------------------------------
	// Member functions								   
	//-------------------------------------------------
	void SetNeighboringTiles(const std::vector<std::vector<TileComponent*>>& vTiles, size_t row, size_t col);
	dae::GameObject* GetNeighboringTile(MovementDirection direction) const { return m_vNeighboringTiles[static_cast<size_t>(direction)]; }

	bool IsCharacterHere(Character character);
	std::pair<Character, dae::GameObject*> GetCharacter(Character character);
	void MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character);
	std::unordered_map<Character, dae::GameObject*> GetCharacters() const { return m_CharactersHere; }

	bool ChangeTile(int currentRound, int& tilesCovered, int stageChange);
	void Reset(int currentRound);

	static int GetMaxTileStage() { return m_MaxTileStage; }

	void AddDiskAsNeighbor(dae::GameObject* pDisk);

private:
	std::vector<dae::GameObject*> m_vNeighboringTiles{};
	std::unordered_map<Character, dae::GameObject*> m_CharactersHere;
	int m_TileStage{ 0 };
	const int m_TileId;
	static int m_MaxTileStage;
	static int m_TileCount;
};


 
