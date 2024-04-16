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
	class LevelManagerComponent;
	enum class ScoreType;
	class ScoreComponent final : public BaseComponent, public Observer<bool>
	{
	public:
		ScoreComponent(GameObject* pGameObject, QbertComponent* pQbertComponent, LevelManagerComponent* pLevelManagerComponent);
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
		virtual void Notify(bool roundFinished) override;
		virtual void SubjectDestroyed(Subject<bool>* pSubject) override;

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
		LevelManagerComponent* m_pLevelManagerComponent{};
		TextComponent* m_pTextComponent{};
	};
}

 
