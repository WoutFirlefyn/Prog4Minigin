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
		virtual void Init() {};
		virtual void Render() const {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void RenderGUI() {};
	protected:
		BaseComponent(GameObject* pGameObject);			// Constructor
		GameObject* GetGameObject() const;
	private:
		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		GameObject* m_pGameObject;

	};
};

 
