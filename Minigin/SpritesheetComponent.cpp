#include <glm/glm.hpp>
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "Texture2D.h"
#include "GameTime.h"

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
	m_CurrCol = cols;
	m_CurrRow = rows;
	m_pGraphicsComponent->SetSourceRect({ static_cast<float>(m_SpriteWidth * m_CurrCol), static_cast<float>(m_SpriteHeight * m_CurrRow), m_SpriteWidth, m_SpriteHeight });
}

void dae::SpritesheetComponent::MoveSourceRectRelative(int cols, int rows)
{
	m_CurrCol += cols;
	m_CurrRow += rows;
	m_pGraphicsComponent->SetSourceRect({ static_cast<float>(m_SpriteWidth * m_CurrCol), static_cast<float>(m_SpriteHeight * m_CurrRow), m_SpriteWidth, m_SpriteHeight });
}