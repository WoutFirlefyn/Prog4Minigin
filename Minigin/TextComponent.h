#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "BaseComponent.h"
namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(GameObject* pGameObject, const std::string& text, std::shared_ptr<Font> pFont);
		TextComponent(GameObject* pGameObject, std::shared_ptr<Font> pFont);
		virtual ~TextComponent() override = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) noexcept = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other)	noexcept = delete;

		virtual void Update() override;
		void SetText(const std::string& text);
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::unique_ptr<Texture2D> m_pTexture;
		std::shared_ptr<Font> m_pFont;
		glm::vec4 m_Color{ 255, 255, 255, 255 };
	};
}

 
