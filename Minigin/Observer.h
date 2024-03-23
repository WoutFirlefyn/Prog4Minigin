#pragma once
#include <vector>
#include <memory>

namespace dae
{
	class TextComponent;
	class GameObject;
	class Observer
	{
	public:
		Observer() = default;				
		virtual ~Observer() = default;

		virtual void Notify(GameObject* pGameObject) = 0;
	};	
	
	class HealthObserver : public Observer
	{
	public:
		HealthObserver(TextComponent* pTextComponent);
		virtual ~HealthObserver() override = default;

		virtual void Notify(GameObject* pGameObject) override;
	private:
		TextComponent* m_pTextComponent;
	};
	
	class ScoreObserver : public Observer
	{
	public:
		ScoreObserver(TextComponent* pTextComponent);
		virtual ~ScoreObserver() override = default;

		virtual void Notify(GameObject* pGameObject) override;
	private:
		TextComponent* m_pTextComponent;
	};

	class Subject
	{
	public:
		void AddObserver(std::unique_ptr<Observer>&& pObserver);
		void RemoveObserver(std::unique_ptr<Observer>&& pObserver);
	protected:
		void NotifyObservers(GameObject* pGameObject);
	private:
		std::vector<std::unique_ptr<Observer>> m_vObservers{};
	};
}
 
