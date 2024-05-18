#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <vector>
#include <unordered_map>

enum class Character;
enum class MovementDirection;
class TileComponent final : public dae::BaseComponent
{
public:
	TileComponent(dae::GameObject* pGameObject);	
	virtual ~TileComponent() override = default;	

	TileComponent(const TileComponent& other)					= delete;
	TileComponent(TileComponent&& other) noexcept				= delete;
	TileComponent& operator=(const TileComponent& other)		= delete;
	TileComponent& operator=(TileComponent&& other)	noexcept	= delete;

	virtual void Init() override;

	bool IsCharacterHere(Character character);
	std::pair<Character, dae::GameObject*> GetCharacter(Character character);
	void MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character);
	std::unordered_map<Character, dae::GameObject*> GetCharacters() const { return m_CharactersHere; }

	bool ChangeTile(int& tilesCovered, int stageChange);
	void Reset();

	static int GetMaxTileStage() { return m_MaxTileStage; }
private:
	std::unordered_map<Character, dae::GameObject*> m_CharactersHere;
	int m_TileStage{ 0 };
	const int m_TileId;
	static int m_MaxTileStage;
	static int m_TileCount;
};


 
