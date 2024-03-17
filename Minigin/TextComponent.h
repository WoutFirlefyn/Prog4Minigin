#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <memory>
#include <string>
#include "BaseComponent.h"

//-----------------------------------------------------
// TextComponent Class									
//-----------------------------------------------------
namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(GameObject* pGameObject, const std::string& text, std::shared_ptr<Font> pFont);				// Constructor
		virtual ~TextComponent() override = default;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) noexcept = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Render() const override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void RenderGUI() override;
		void SetText(const std::string& text);

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture;

	};
}

 
