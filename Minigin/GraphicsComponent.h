#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class Texture2D;

	class GraphicsComponent final : public BaseComponent
	{
	public:
		GraphicsComponent(GameObject* pGameObject, const std::string& name, bool renderingEnabled = true);
		GraphicsComponent(GameObject* pGameObject);		
		virtual ~GraphicsComponent() override = default;
 
		GraphicsComponent(const GraphicsComponent& other) = delete;
		GraphicsComponent(GraphicsComponent&& other) noexcept = delete;
		GraphicsComponent& operator=(const GraphicsComponent& other) = delete;
		GraphicsComponent& operator=(GraphicsComponent&& other)	noexcept = delete;

		virtual void Render() const override;
		void SetTexture(const std::string& filename);
		void SetTexture(Texture2D* pTexture);
		void ToggleRendering(bool isEnabled) { m_RenderingEnabled = isEnabled; }
		Texture2D* GetTexture() const { return m_pTexture; }
		void ToggleSourceRect(bool useSourceRect) { m_UseSourceRect = useSourceRect; }
		void SetSourceRect(const glm::vec4& srcRect) { m_SrcRect = srcRect; }
	private:
		Texture2D* m_pTexture{};
		glm::vec4 m_SrcRect{};
		bool m_RenderingEnabled{ true };
		bool m_UseSourceRect{ false };
	};
}
 
