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
		ScoreComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);
		virtual ~ScoreComponent() override;

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
		virtual void SubjectDestroyed(Subject<>* pSubject) override;

		int GetScore() const { return m_Score; }

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		void UpdateText();

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		int m_Score{ 0 };
		QbertComponent* m_pQbertComponent{};
		TextComponent* m_pTextComponent{};
	};
}

 
