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
namespace dae
{
	class SpritesheetComponent;
}
enum class Character;
enum class MovementState;
enum class MovementDirection;
class QbertComponent;
class DiskComponent final : public dae::BaseComponent
{
public:
	DiskComponent(dae::GameObject* pGameObject, dae::GameObject* pTopTile);
	~DiskComponent() = default;			

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
	virtual void Init() override;
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
	dae::SpritesheetComponent* m_pSpritesheetComponent;
	glm::vec3 m_StartPos{};
	float m_PlatformLerpValue{ 0.f };
	const float m_TimeToReachTop{ 2.5f };
	float m_AccumSec{};
	float m_Fps{ 12.f };
};

 
