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
}
class TileComponent;
enum class Character;
enum class MovementState;
enum class MovementDirection;
class LevelManagerComponent final : public dae::BaseComponent, public dae::Observer<Character, MovementState, MovementDirection>, public dae::Observer<bool>
{
public:
	LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene);				// Constructor
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
	void SpawnQbert(dae::GameObject* pGameObject); // Remove this function, this is be temporary (or at least improve it a bit)

	virtual void Init() override;

	void AddObserver(dae::Subject<Character, MovementState, MovementDirection>* pMoveStateChanged);
	virtual void Notify(Character character, MovementState movementState, MovementDirection movementDirection) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementState, MovementDirection>* pSubject) override;

	virtual void Notify(bool roundFinished) override;

	std::unique_ptr<dae::Subject<bool>> TileChanged;
	std::unique_ptr<dae::Subject<Character>> CharacterFell;
private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	bool AreAllTilesCovered() const;
	dae::GameObject* FindCharacter(Character character) const;

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::Subject<Character, MovementState, MovementDirection>* m_pMoveStateChangedSubject;
	std::vector<dae::GameObject*> m_vTiles;
	int m_LevelLength{ 7 };
	int m_TilesCovered{ 0 };
	int m_CurrentRound{ 0 };
	int m_AmountOfDisks{ 2 };
};

