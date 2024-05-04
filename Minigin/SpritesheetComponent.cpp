//---------------------------
// Includes
//---------------------------
#include <glm/glm.hpp>
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "Texture2D.h"
#include "GameTime.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::SpritesheetComponent::SpritesheetComponent(GameObject* pGameObject, int spriteCols, int spriteRows) : BaseComponent(pGameObject)
{
	m_pGraphicsComponent = GetGameObject()->GetComponent<GraphicsComponent>();
	auto size = m_pGraphicsComponent->GetTexture()->GetSize();
	m_SpriteWidth = size.x / spriteCols;
	m_SpriteHeight = size.y / spriteRows;
	m_pGraphicsComponent->ToggleSourceRect(true);
	m_pGraphicsComponent->SetSourceRect({ 0, 0, m_SpriteWidth, m_SpriteHeight });
}

void dae::SpritesheetComponent::MoveSourceRect(int cols, int rows)
{
	m_pGraphicsComponent->SetSourceRect({ static_cast<float>(m_SpriteWidth * cols), static_cast<float>(m_SpriteHeight * rows), m_SpriteWidth, m_SpriteHeight });
	m_CurrCol = cols;
	m_CurrRow = rows;
}




