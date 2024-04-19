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
	enum class Character;
	enum class MovementState;
	enum class MovementDirection;
	class QbertComponent;
	class DiskComponent final : public BaseComponent
	{
	public:
		DiskComponent(GameObject* pGameObject, GameObject* pTopTile);
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
		virtual void Init() override;
		virtual void Update() override;

		std::pair<Character, GameObject*> GetCharacter() const;
		void MoveCharacterHere(const std::pair<Character, GameObject*>& character);
	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		GameObject* m_pTopTile;
		std::pair<Character, GameObject*> m_pCharacter;
		glm::vec3 m_StartPos{};
		float m_AccumSec{};
		float m_TimeToReachTop{ 5.f };
	};
}

 
