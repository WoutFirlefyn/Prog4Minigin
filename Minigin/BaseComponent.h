#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------

//-----------------------------------------------------
// BaseComponent Class									
//-----------------------------------------------------
namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		BaseComponent() = default;				// Constructor
		virtual ~BaseComponent() = default;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) noexcept = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Update(GameObject* pGameObject) = 0;

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------


	};
};

 
