#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// ScoreComponent Class									
//-----------------------------------------------------
namespace dae
{
	class TextComponent;
	class QbertComponent;
	enum class ScoreType;
	class ScoreComponent final : public BaseComponent, public Observer<>
	{
	public:
		ScoreComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);				// Constructor
		virtual ~ScoreComponent() override;			// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) noexcept = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Notify() override;
		virtual void SubjectDestroyed() override;

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		void UpdateText();

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		QbertComponent* m_pQbertComponent{};
		TextComponent* m_pTextComponent{};
	};
}

 
