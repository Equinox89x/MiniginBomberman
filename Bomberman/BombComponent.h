#pragma once
#include "Component.h"
#include "MathLib.h"
#include "PathwayCreatorComponent.h"
#include "Scene.h"
#include "States.h"
#include "TextureComponent.h"

namespace dae
{
	class BombComponent final : public Component

	{
	public:
		BombComponent(Scene* scene, GameObject* bombDropper)
			: m_Scene{ scene },
			  BombDropper{ bombDropper } {

			  };
		~BombComponent() = default;
		BombComponent(const BombComponent&) = delete;
		BombComponent(BombComponent&&) noexcept = delete;
		BombComponent& operator=(const BombComponent&) = delete;
		BombComponent& operator=(BombComponent&&) noexcept = delete;

		virtual void Update() override;
		virtual void Render() const override;
		virtual void Init() override;

		void StartBomb(int tileId, int bombStrength);

		void SetState(EntityState* playerState, MathLib::EBombState lifeState)
		{
			if (playerState)
			{
				if (m_BombState)
					m_BombState->OnEnd(GetGameObject());
				m_BombState = std::unique_ptr<EntityState>(playerState);
				m_State = lifeState;
				if (m_BombState)
					m_BombState->OnStart(GetGameObject());
			}
		}
		MathLib::EBombState GetState() { return m_State; };
		EntityState*		GetBombState() { return m_BombState.get(); };
		int					GetBombStrength() { return m_BombStrength; };
		int					GetTileId() { return m_TileId; };

		GameObject* BombDropper{ nullptr };
		float		LifeTime{ 0 };

	private:
		std::unique_ptr<EntityState> m_BombState{ nullptr };
		MathLib::EBombState			 m_State{ MathLib::EBombState::Fuse };
		Scene*						 m_Scene{ nullptr };
		int							 m_BombStrength{ 1 };
		int							 m_TileId{ 0 };
	};
} // namespace dae
