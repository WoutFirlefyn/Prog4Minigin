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
}

void dae::SpritesheetComponent::Init()
{
	m_pGraphicsComponent = GetGameObject()->GetComponent<GraphicsComponent>();
	auto pTexture = m_pGraphicsComponent->GetTexture();
	auto size = pTexture->GetSize();
	m_SpriteWidth = size.x / m_SpriteCols;
	m_SpriteHeight = size.y / m_SpriteRows;
	pTexture->SetSourceRect(0, 0, m_SpriteWidth, m_SpriteHeight);
}

void dae::SpritesheetComponent::Update()
{
}

//---------------------------
// Member functions
//---------------------------

// write member functions here



