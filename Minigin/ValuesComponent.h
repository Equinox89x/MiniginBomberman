#pragma once
#include "Component.h"
#include "Scene.h"
#include "Subject.h"

namespace dae
{

	class ValuesComponent final : public Component, public Subject
	{

	public:
		ValuesComponent(Scene* scene) : m_Scene{ scene } {};
		~ValuesComponent() = default;
		ValuesComponent(const ValuesComponent&) = delete;
		ValuesComponent(ValuesComponent&&) noexcept = delete;
		ValuesComponent& operator=(const ValuesComponent&) = delete;
		ValuesComponent& operator=(ValuesComponent&&) noexcept = delete;

		void Update() {};
		virtual void FixedUpdate() override {};
		void Render() const {};

		int GetLives() const;
		int GetScores() const;
		int GetShotsFired() { return NrOfShotsFired; };
		int GetNumberOfHits() { return NrOfHits; };

		void SetLives(int lives);
		void SetName(std::string name) { m_ComponentName = name; };

		void Damage();
		void IncreaseScore(int score);
		void ResetObserver();
		void IncreaseHits() { NrOfHits++; };
		void IncreaseShots() { NrOfShotsFired++; };

		void Reset();
		void GameEnd();


	private:
		int m_Lives{ 3 };
		int m_Score{ 0 };
		std::string m_ComponentName;

		Scene* m_Scene;
		int NrOfShotsFired{ 0 }, NrOfHits{ 0 };
	};
}

