//---------------------------
// Includes
//---------------------------
#include "TileSpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "Texture2D.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::TileSpritesheetComponent::TileSpritesheetComponent(GameObject* pGameObject, int spriteCols, int spriteRows) : SpritesheetComponent(pGameObject, spriteCols, spriteRows)
{
}

void dae::TileSpritesheetComponent::MoveSourceRect(int cols, int rows)
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

// write member functions here




