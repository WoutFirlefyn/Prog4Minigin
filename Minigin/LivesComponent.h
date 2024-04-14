#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// QbertComponent Class									
//-----------------------------------------------------
namespace dae
{
	class QbertComponent;
	class LivesComponent final : public BaseComponent, public Observer<>
	{
	public:
		LivesComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);				// Constructor
		virtual ~LivesComponent() override;			// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) noexcept = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Notify() override;
		virtual void SubjectDestroyed(Subject<>* pSubject) override;

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		QbertComponent* m_pQbertComponent{};
		static int m_CurrentId;
		const int m_HeartId;
	};
}


