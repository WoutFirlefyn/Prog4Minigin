#pragma once
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include "Transform.h"
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaTime);
		void Render() const;
		
		Transform GetTransform() const;
		void SetPosition(float x, float y);
		template <typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			m_vComponents.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
		}

		template<typename T>
		void RemoveComponent() 
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must be a subclass of BaseComponent");
			auto it = std::remove_if(m_vComponents.begin(), m_vComponents.end(), [](std::shared_ptr<BaseComponent> pComponent) 
			    {
			        return dynamic_cast<T*>(pComponent.get()) != nullptr;
				});
			assert(it != m_vComponents.end() && "Component to remove not found");
			m_vComponents.erase(it, m_vComponents.end());
		}

		template<typename T>
		T* GetComponent() const 
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must be a subclass of BaseComponent");
			auto it = std::ranges::find_if(m_vComponents, [](std::shared_ptr<BaseComponent> pComponent) 
				{
					return dynamic_cast<T*>(pComponent.get()) != nullptr;
				});

			assert(it != m_vComponents.end() && "Component to get not found");

			return static_cast<T*>((*it).get());
		}

		template<typename T>
		bool CheckComponent() const 
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must be a subclass of BaseComponent");
			auto it = std::ranges::find_if(m_vComponents, [](std::shared_ptr<BaseComponent> pComponent) 
				{
					return dynamic_cast<T*>(pComponent.get()) != nullptr;
				});

			return it != m_vComponents.end();
		}

	private:
		Transform m_Transform{};
		std::vector<std::shared_ptr<BaseComponent>> m_vComponents;
	};
}
