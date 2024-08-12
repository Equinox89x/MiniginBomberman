#pragma once
#include "Component.h"
#include "MathLib.h"
#include "Scene.h"
#include "States.h"
#include "Subject.h"
#include "TextureComponent.h"

namespace dae
{
	class PlayerComponent final : public Component, public Subject
	{
	public:
		PlayerComponent(Scene* scene, glm::vec2 startPos) : m_Scene{ scene }, m_OriginalPosition{ startPos } {};
		~PlayerComponent();
		PlayerComponent(const PlayerComponent&) = delete;
		PlayerComponent(PlayerComponent&&) noexcept = delete;
		PlayerComponent& operator=(const PlayerComponent&) = delete;
		PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() const override{};

		void Reposition();
		void ActivateUnderlyingThing(PathWay& pathway);

		void SetState(EntityState* playerState, MathLib::ELifeState lifeState)
		{
			if (playerState)
			{
				if (m_PlayerState)
					m_PlayerState->OnEnd(GetGameObject());
				m_PlayerState = std::unique_ptr<EntityState>(playerState);
				m_State = lifeState;
				if (m_PlayerState)
					m_PlayerState->OnStart(GetGameObject());
			}
		}
		
		bool				HasDetonator{ false };
		MathLib::ELifeState GetState() { return m_State; };
		EntityState*		GetPlayerState() { return m_PlayerState.get(); };
		int					GetBombStrength() { return m_BombStrength; };
		int					GetMaxBombs() { return m_MaxBombs; };
		GameObject*			GetOldestBomb(std::string playerName);


	private:
		Scene*						 m_Scene{ nullptr };
		std::unique_ptr<EntityState> m_PlayerState{ nullptr };
		MathLib::ELifeState			 m_State{ MathLib::ELifeState::ALIVE };

		glm::vec2 m_OriginalPosition;
		int		  m_BombStrength{ 1 };
		int		  m_MaxBombs{ 1 };
	};
} // namespace dae
