#pragma once
#include "Event.h"

namespace dae {
	class GameObject;
	class Observer
	{
	public:
		explicit Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;
		virtual void Notify(GameObject* go, Event& event) = 0;
	};
}