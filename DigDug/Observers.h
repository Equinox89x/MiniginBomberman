#pragma once
#include "Observer.h"
#include "vector"
#include "Event.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <functional>
//#include "Steam.h"
//#include "SteamHolder.h"

namespace dae
{
	class HealthObserver final : public Observer
	{
	public:
		HealthObserver(Scene* scene, int id) : m_Scene{ scene }, m_Id{id} {};
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;
		int m_Id{ 0 };
	};

	class ScoreObserver final : public Observer
	{
	public:

		ScoreObserver(Scene* const scene, int id) : m_Scene{ scene }, m_Id{ id } {};
		~ScoreObserver();
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;
		int m_Id{ 0 };

	};

	class EnemyDeathObserver final : public Observer
	{
	public:

		EnemyDeathObserver(Scene* const scene) : m_Scene{ scene } {};
		~EnemyDeathObserver() {};
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;

	};

	class RockDeathObserver final : public Observer
	{
	public:

		RockDeathObserver(Scene* const scene) : m_Scene{ scene } {};
		~RockDeathObserver() {};
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;

	};
	
	//class PumpEnemyOverlapObserver final : public Observer
	//{
	//public:

	//	PumpEnemyOverlapObserver(Scene* const scene) : m_Scene { scene } {};
	//	void Notify(GameObject* go, Event& event) override;

	//private:

	//	Scene* m_Scene;

	//};
}