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

		void Update();
		void FixedUpdate();
		void Render() const;
		
		void SetPosition(float x, float y);
		Transform GetTransform() const { return m_Transform; }

		void MarkAsDestroyed() { m_IsDestroyed = true; }
		bool IsDestroyed() const { return m_IsDestroyed; }

		GameObject* GetParent() const { return m_pParent; }
		void SetParent(GameObject* pParent);
		size_t GetChildCount() const { return m_vChildren.size(); }
		GameObject* GetChildAtIdx(int idx) const { return m_vChildren[idx]; }

#pragma region Components
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
#pragma endregion

	private:
		//void AddChild(GameObject* pChild);
		//void RemoveChild(GameObject* pChild);

		GameObject* m_pParent{};
		std::vector<GameObject*> m_vChildren{};

		Transform m_Transform{};
		std::vector<std::shared_ptr<BaseComponent>> m_vComponents;
		bool m_IsDestroyed{ false };
	};
}
