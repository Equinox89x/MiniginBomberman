#pragma once
#include <vector>
#include "Observer.h"
#include "Event.h"
#include <memory>


namespace dae
{
	class Observer;
	class GameObject;

	class Subject
	{
	public:
		void AddObserver(std::shared_ptr<Observer> observer)
		{
			m_Callbacks.push_back(observer);
		}

		void RemoveObserver(std::shared_ptr<Observer> observer)
		{
			auto it = std::find(m_Callbacks.begin(), m_Callbacks.end(), observer);
			m_Callbacks.erase(it);
		}

		void Notify(GameObject* go, Event& event)
		{
			for (auto& observer : m_Callbacks)
			{
				observer->Notify(go, event);
			}
		}

	private:
		std::vector<std::shared_ptr<Observer>> m_Callbacks{};
	};
}