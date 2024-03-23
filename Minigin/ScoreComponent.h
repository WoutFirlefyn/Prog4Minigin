#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// HealthComponent Class									
//-----------------------------------------------------
namespace dae
{
	class ScoreComponent final : public BaseComponent, public Subject
	{
	public:
		ScoreComponent(GameObject* pGameObject);				// Constructor
		virtual ~ScoreComponent() override = default;			// Destructor

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
		virtual void Render() const override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void RenderGUI() override;

		void IncreaseScore(int scoreIncrease);
		int GetScore() const { return m_Score; }

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		int m_Score{};
	};
}

 
