#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <vector>
#include <memory>
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// LevelManagerComponent Class									
//-----------------------------------------------------
namespace dae
{
	class Scene;
	class TileComponent;
	class QbertComponent;
	enum class Character;
	enum class MovementState;
	enum class MovementDirection;
	class LevelManagerComponent final : public BaseComponent, public Observer<Character, MovementState, MovementDirection>, public Observer<bool>
	{
	public:
		LevelManagerComponent(GameObject* pGameObject, QbertComponent* pQbertComponent, Scene& scene);				// Constructor
		~LevelManagerComponent();				// Destructor

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
		virtual void Init() override;

		virtual void Notify(Character character, MovementState movementState, MovementDirection movementDirection) override;
		virtual void SubjectDestroyed(Subject<Character, MovementState, MovementDirection>* pSubject) override;

		virtual void Notify(bool roundFinished) override;

		std::unique_ptr<Subject<bool>> TileChanged;
	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		bool AreAllTilesCovered() const;
		GameObject* FindCharacter(Character character) const;

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		QbertComponent* m_pQbertComponent;
		std::vector<GameObject*> m_vTiles;
		int m_LevelLength{ 7 };
		int m_TilesCovered{ 0 };
		int m_CurrentRound{ 0 };
		int m_AmountOfDisks{ 2 };
	};
}
