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
		TextComponent(GameObject* pGameObject, std::shared_ptr<Font> pFont, const std::string& text = "");
		virtual ~TextComponent() override = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) noexcept = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other)	noexcept = delete;

		virtual void Update() override;
		void SetText(const std::string& text);
		void SetColor(const glm::vec4& color);
	private:
		bool m_NeedsUpdate{ false };
		std::string m_Text{ "" };
		std::unique_ptr<Texture2D> m_pTexture{ nullptr };
		std::shared_ptr<Font> m_pFont{ nullptr };
		glm::vec4 m_Color{ 255, 255, 255, 255 };
	};
}

 
