#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Transform.h"
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update(float deltaTime);
		virtual void Render() const;
		
		void SetTexture(const std::string& filename);
		Transform GetPosition() const;
		void SetPosition(float x, float y);
		void AddComponent(std::shared_ptr<BaseComponent> pComponent);
		void RemoveComponent(std::shared_ptr<BaseComponent> pComponent);
		void GetComponent() const;

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::shared_ptr<BaseComponent>> m_vComponents;
	};
}
