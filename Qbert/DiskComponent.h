#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"
#include "glm/glm.hpp"

//-----------------------------------------------------
// DiscComponent Class									
//-----------------------------------------------------
enum class Character;
enum class MovementState;
enum class MovementDirection;
class QbertComponent;
class DiskComponent final : public dae::BaseComponent
{
public:
	DiskComponent(dae::GameObject* pGameObject, dae::GameObject* pTopTile);
	~DiskComponent();			

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	DiskComponent(const DiskComponent& other) = delete;
	DiskComponent(DiskComponent&& other) noexcept = delete;
	DiskComponent& operator=(const DiskComponent& other) = delete;
	DiskComponent& operator=(DiskComponent&& other)	noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Update() override;

	std::pair<Character, dae::GameObject*> GetCharacter() const;
	void MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character);
private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::GameObject* m_pTopTile;
	std::pair<Character, dae::GameObject*> m_pCharacter;
	glm::vec3 m_StartPos{};
	float m_AccumSec{};
	float m_TimeToReachTop{ 2.5f };
};

 
