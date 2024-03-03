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
		BaseComponent() = default;
		BaseComponent(GameObject* pGameObject);			// Constructor
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
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void Render() const = 0;
	protected:
		GameObject* GetGameObject() const;
	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		GameObject* m_pGameObject;

	};
};

 
