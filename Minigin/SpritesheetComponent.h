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
	class Texture2D;
	class GraphicsComponent;
	class SpritesheetComponent final : public BaseComponent
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
		SpritesheetComponent& operator=(SpritesheetComponent&& other) noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		void MoveSourceRect(int cols, int rows);
		int GetCurrRow() const { return m_CurrRow; } 
		int GetCurrCol() const { return m_CurrCol; } 

	private:
		GraphicsComponent* m_pGraphicsComponent{ nullptr };
		int m_SpriteWidth{};
		int m_SpriteHeight{};
		int m_CurrRow{ 0 };
		int m_CurrCol{ 0 };
	};
}

 
