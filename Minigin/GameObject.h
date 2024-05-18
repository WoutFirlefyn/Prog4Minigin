#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <stdexcept>
#include <execution>
#include <iostream>
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

		void Init();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderGUI();
		
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec3& pos);
		glm::vec3 GetLocalPosition() { return m_LocalTransform.GetPosition(); }
		glm::vec3 GetWorldPosition() { return GetWorldTransform().GetPosition(); }

		void SetScale(float x, float y);
		void SetScale(const glm::vec3& scale);
		glm::vec3 GetLocalScale() { return m_LocalTransform.GetScale(); }
		glm::vec3 GetWorldScale() { return GetWorldTransform().GetScale(); }

		void SetLocalTransform(const dae::Transform& transform);
		const Transform& GetLocalTransform() { return m_LocalTransform; }
		const Transform& GetWorldTransform();
		void UpdateWorldTransform();

		void Destroy() { m_IsDestroyed = true; }
		bool IsDestroyed() const { return m_IsDestroyed; }

		GameObject* GetParent() const { return m_pParent; }
		void SetParent(GameObject* pParent, bool keepWorldPosition = false);
		size_t GetChildCount() const { return m_vChildren.size(); }
		GameObject* GetChildAtIdx(int idx) const { return m_vChildren[idx]; }

#pragma region Components
		template <typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must be a subclass of BaseComponent");
			m_Components.insert(std::make_pair(std::type_index(typeid(T)), std::make_unique<T>(this, std::forward<Args>(args)...)));
		}

		template<typename T>
		void RemoveComponent() 
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must be a subclass of BaseComponent");
			m_Components.erase(typeid(T));
		}

		template<typename T>
		T* GetComponent() const
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must be a subclass of BaseComponent");

			if (HasComponent<T>())
				return dynamic_cast<T*>((*m_Components.find(typeid(T))).second.get());

			return nullptr;
		}

		template<typename T>
		bool HasComponent() const 
		{
			static_assert(std::is_base_of<BaseComponent, T>::value, "T must be a subclass of BaseComponent");
			return m_Components.contains(typeid(T));
		}
#pragma endregion

	private:
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);
		bool IsChild(GameObject* pGameObject) const;
		void SetPositionDirty();

		GameObject* m_pParent{};
		std::vector<GameObject*> m_vChildren{};
		std::unordered_map<std::type_index, std::unique_ptr<BaseComponent>> m_Components{};

		Transform m_WorldTransform{};
		Transform m_LocalTransform{};
		bool m_PositionIsDirty{ false };

		bool m_IsDestroyed{ false };
	};
}
