#pragma once
#include <vector>
#include <memory>

namespace dae
{
	template <typename... Args>
	class Subject;

	template <typename... Args>
	class Observer
	{
	public:
		Observer() = default;				
		virtual ~Observer() = default;

		virtual void Notify(Args... args) = 0;
		virtual void SubjectDestroyed(Subject<Args...>*) {}
	};	

	template <typename... Args>
	class Subject final
	{
	public:
		Subject() = default;
		~Subject()
		{
			for (auto& pObserver : m_vObservers)
				pObserver->SubjectDestroyed(this);
		} 

		void AddObserver(Observer<Args...>* pObserver)
		{
			if (pObserver)
				m_vObservers.push_back(pObserver);
		}
		void RemoveObserver(Observer<Args...>* pObserver)
		{
			m_vObservers.erase(std::remove(m_vObservers.begin(), m_vObservers.end(), pObserver), m_vObservers.end());
		}
		void NotifyObservers(Args... args)
		{
			for (auto& pObserver : m_vObservers)
				pObserver->Notify(args...);
			//for (int i{}; i < m_vObservers.size(); ++i)
			//	m_vObservers[i]->Notify(args...);
		}
	private:
		std::vector<Observer<Args...>*> m_vObservers{};
	};
}
 
