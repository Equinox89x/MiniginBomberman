#pragma once
#include "Event.h"
#include "GameObject.h"
#include "Observer.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "vector"
#include <functional>
// #include "Steam.h"
// #include "SteamHolder.h"

namespace dae
{
	class HealthObserver final : public Observer
	{
	public:
		HealthObserver(Scene* scene, int id) : m_Scene{ scene }, m_Id{ id } {};
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;
		int	   m_Id{ 0 };
	};

	class ScoreObserver final : public Observer
	{
	public:
		ScoreObserver(Scene* const scene, int id) : m_Scene{ scene }, m_Id{ id } {};
		~ScoreObserver();
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;
		int	   m_Id{ 0 };
	};

	class DoorObserver final : public Observer
	{
	public:
		DoorObserver(Scene* const scene) : m_Scene{ scene } {};
		~DoorObserver(){};
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;
	};

	// class PumpEnemyOverlapObserver final : public Observer
	//{
	// public:

	//	PumpEnemyOverlapObserver(Scene* const scene) : m_Scene { scene } {};
	//	void Notify(GameObject* go, Event& event) override;

	// private:

	//	Scene* m_Scene;

	//};
} // namespace dae