//---------------------------
// Includes
//---------------------------
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "Texture2D.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::SpritesheetComponent::SpritesheetComponent(GameObject* pGameObject, int spriteCols, int spriteRows) : BaseComponent(pGameObject)
	, m_SpriteCols{ spriteCols }
	, m_SpriteRows{ spriteRows }
{
	m_pTexture = GetGameObject()->GetComponent<GraphicsComponent>()->GetTexture();
	auto size = m_pTexture->GetSize();
	m_SpriteWidth = size.x / m_SpriteCols;
	m_SpriteHeight = size.y / m_SpriteRows;
	m_pTexture->SetSourceRect(0, 0, m_SpriteWidth, m_SpriteHeight);
}

void dae::SpritesheetComponent::Init()
{
}

void dae::SpritesheetComponent::Update()
{
}

void dae::SpritesheetComponent::MoveSourceRect(int cols, int rows)
{
	if (m_pTexture)
	{
		auto srcRectSize = m_pTexture->GetSourceRectSize();
		m_pTexture->SetSourceRect(srcRectSize.x * cols, srcRectSize.y * rows);
	}
}

//---------------------------
// Member functions
//---------------------------





