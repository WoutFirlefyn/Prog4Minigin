#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"

//-----------------------------------------------------
// SpritesheetComponent Class									
//-----------------------------------------------------
namespace dae
{
	class GraphicsComponent;
	class SpritesheetComponent : public BaseComponent
	{
	public:
		SpritesheetComponent(GameObject* pGameObject, int spriteCols, int spriteRows);
		~SpritesheetComponent() = default;			

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		SpritesheetComponent(const SpritesheetComponent& other) = delete;
		SpritesheetComponent(SpritesheetComponent&& other) noexcept = delete;
		SpritesheetComponent& operator=(const SpritesheetComponent& other) = delete;
		SpritesheetComponent& operator=(SpritesheetComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Update() override;


	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		GraphicsComponent* m_pGraphicsComponent{ nullptr };
		int m_SpriteWidth{};
		int m_SpriteHeight{};
		int m_SpriteRows{};
		int m_SpriteCols{};
	};
}

 
