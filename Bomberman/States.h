#pragma once
#include "GameObject.h"
#include <glm/ext/vector_float2.hpp>
#include <map>

namespace dae
{
	struct PathWay;
	class Scene;
	class EntityState
	{
	public:
		explicit EntityState(Scene* scene) : m_Scene{ scene } {};
		virtual ~EntityState() = default;

		EntityState(const EntityState& other) = delete;
		EntityState(EntityState&& other) noexcept = delete;
		EntityState& operator=(const EntityState& other) = delete;
		EntityState& operator=(EntityState&& other) noexcept = delete;
		virtual void OnStart(GameObject* /*pGameObject*/){};
		virtual void Update(GameObject* pGameObject) = 0;
		virtual void OnEnd(GameObject* /*pGameObject*/){};

	protected:
		Scene* m_Scene{ nullptr };
	};

	class AliveState : public EntityState
	{
	public:
		AliveState(Scene* scene) : EntityState(scene){};
		void OnStart(GameObject* pGameObject) override;
		void Update(GameObject* pGameObject) override;

	private:
	};

	class InvincibleState : public EntityState
	{
	public:
		InvincibleState(Scene* scene) : EntityState(scene){};
		void OnStart(GameObject* pGameObject) override;
		void Update(GameObject* pGameObject) override;

	private:
		float m_timer{ 3.f };
	};

	class BombedState : public EntityState
	{
	public:
		BombedState(Scene* scene) : EntityState(scene){};
		void OnStart(GameObject* pGameObject) override;
		void Update(GameObject* pGameObject) override;

	private:
		float m_DeathTimer{ 1.3f };
	};

	class DeathState : public EntityState
	{
	public:
		DeathState(Scene* scene) : EntityState(scene){};
		void OnStart(GameObject* pGameObject) override;
		void Update(GameObject* pGameObject) override;

	private:
		bool m_hasDeathSequence{ true };
		float m_DeathTimer{ 0.7f };
	};

	class FuseState : public EntityState
	{
	public:
		FuseState(Scene* scene) : EntityState(scene){};
		void OnStart(GameObject* pGameObject) override;
		void Update(GameObject* pGameObject) override;

	private:
		float m_FuseTimer{ 3.f };
	};

	class ExplosionState : public EntityState
	{
	public:
		ExplosionState(Scene* scene) : EntityState(scene){};
		void OnStart(GameObject* pGameObject) override;
		void Update(GameObject* pGameObject) override;

	private:
		float m_ExplodeTimer{ 0.25f };
		int	  m_BombStrength{ 1 };
		int	  m_TileId{ 0 };
		bool  m_HitWallLeft{ false }, m_HitWallRight{ false }, m_HitWallTop{ false }, m_HitWallBottom{ false };

		void HandleExplosionPlacement(int& index, const std::map<int, PathWay>& pathways, bool& outHitWall, float rotationOffset = 0.f);
		void HandleExplosionEnd(int& index);
	};

} // namespace dae