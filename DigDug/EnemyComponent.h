#pragma once
#include "Component.h"
#include "MathLib.h"
#include "Scene.h"
#include "States.h"
#include "Subject.h"
#include "TextureComponent.h"

namespace dae
{
	class EnemyComponent final : public Component, public Subject
	{
	public:
		EnemyComponent(Scene* scene, MathLib::FEnemyStats enemyStats, bool isVersus = false) : m_Scene{ scene }, m_EnemyStats{ enemyStats }, m_IsVersus{ isVersus } {};
		~EnemyComponent();
		EnemyComponent(const EnemyComponent&) = delete;
		EnemyComponent(EnemyComponent&&) noexcept = delete;
		EnemyComponent& operator=(const EnemyComponent&) = delete;
		EnemyComponent& operator=(EnemyComponent&&) noexcept = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() const override;

		void SetState(EntityState* playerState, MathLib::ELifeState lifeState)
		{
			if (playerState)
			{
				if (m_EnemyState)
					m_EnemyState->OnEnd(GetGameObject());
				m_EnemyState = std::unique_ptr<EntityState>(playerState);
				m_State = lifeState;
				if (m_EnemyState)
					m_EnemyState->OnStart(GetGameObject());
			}
		}
		MathLib::ELifeState	  GetState() { return m_State; };
		EntityState*		  GetEnemyState() { return m_EnemyState.get(); };
		MathLib::FEnemyStats& GetEnemyStats() { return m_EnemyStats; };

		void		SetPlayer(GameObject* player) { m_Player = player; }
		GameObject* GetPlayer() { return m_Player; };

	private:
		Scene*						 m_Scene{ nullptr };
		std::unique_ptr<EntityState> m_EnemyState{ nullptr };
		MathLib::ELifeState			 m_State{ MathLib::ELifeState::ALIVE };
		bool						 m_IsVersus{ false };

		GameObject*			 m_Player{ nullptr };
		MathLib::FEnemyStats m_EnemyStats;
	};
} // namespace dae
