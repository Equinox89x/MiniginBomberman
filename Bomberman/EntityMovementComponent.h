#pragma once
#include "Component.h"
#include "MathLib.h"
#include "PathwayCreatorComponent.h"
#include "Scene.h"
#include "TextureComponent.h"
#include <map>
#include "EnemyComponent.h"

namespace dae
{
	class EntityMovementComponent final : public Component
	{

	public:
		EntityMovementComponent(Scene* scene, glm::vec2 startPos, bool isAutonomous = false) : m_Scene{ scene }, m_StartPos{ startPos }, m_IsAutonomous{ isAutonomous } { };
		~EntityMovementComponent()
		{
			//delete m_CachedLocation;
			//m_CachedLocation = nullptr;
		};
		EntityMovementComponent(const EntityMovementComponent&) = delete;
		EntityMovementComponent(EntityMovementComponent&&) noexcept = delete;
		EntityMovementComponent& operator=(const EntityMovementComponent&) = delete;
		EntityMovementComponent& operator=(EntityMovementComponent&&) noexcept = delete;

		virtual void Update() override;
		void		 HandleSimpleMovement();
		virtual void Render() const override;
		virtual void Init() override
		{
			if (auto comp{ GetGameObject()->GetComponent<EnemyComponent>() })
			{
				m_Stats = comp->GetEnemyStats();
			}
		};

		void SetIsController(bool isController) { m_IsController = isController; };
		void SetStartPos(glm::vec2 startPos) { m_StartPos = startPos; };
		void Reposition() { GetGameObject()->GetTransform()->SetPosition(m_StartPos.x, m_StartPos.y); };

		void SetMovement(MathLib::EMovement movement);

		MathLib::ELifeState GetState() { return m_PlayerState; };
		SDL_Rect			GetCollider() { return m_PathwayColliderRect; };
		SDL_Rect			GetCharacterCollider() { return m_Rect; };

		void SetNextTileId(int id) { m_NextTileId = id; };
		void SetCurrentTileId(int id);
		int	 GetNextTileId() { return m_NextTileId; };
		int	 GetCurrentTileId() { return m_CurrentTileId; };
		void CheckMovement(const std::map<int, PathWay>& pathways);

		void			   DisableMovement(bool isDisabled) { m_CanMove = !isDisabled; };
		MathLib::EMovement GetDirection() { return m_Movement; };
		//std::string		   GetLastDirection() { return m_LastDir; }
		//void			   SetLastDirection(std::string dir) { m_LastDir = dir; }
		SDL_Rect GetPathCollider(MathLib::EMovement movement);

		void		SetEnemyName(std::string enemyName) { m_EnemyName = enemyName; }
		std::string GetEnemyName() { return m_EnemyName; };

	private:
		bool							  m_IsController{ false }, m_IsAutonomous{ false }, m_CanMove{ true };
		int								  m_NextTileId{ 0 }, m_CurrentTileId{ 0 };
		std::map<MathLib::EMovement, int> m_CurrentSurroundingTiles{};
		Scene*							  m_Scene{ nullptr };

		SDL_Rect m_BottomRect{}, m_TopRect{}, m_Rect{}, m_LeftRect{}, m_RightRect{}, m_PathwayColliderRect{};

		MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };
		MathLib::EMovement	m_Movement{ MathLib::EMovement::DOWN };
		MathLib::EMovement	m_PrevMovement{};
		glm::vec2			m_StartPos{};
		glm::vec2			m_PrevLoc{};

		// Autonomous
		float								  m_MoveTimer{ 2 };
		SDL_Rect							  m_EnemyDetectionOverlapper{};
		MathLib::EMovingState				  m_State{ MathLib::EMovingState::MovingLeft };
		std::map<MathLib::EMovingState, bool> m_MovementDir{
			std::make_pair(MathLib::EMovingState::MovingUp, true),
			std::make_pair(MathLib::EMovingState::MovingLeft, true),
			std::make_pair(MathLib::EMovingState::MovingRight, true),
			std::make_pair(MathLib::EMovingState::MovingDown, true),
		};
		std::string m_EnemyName{ "Balloom" };
		//std::string m_LastDir{ "Left" };
		//PathWay*	m_CachedLocation{ nullptr };
		//PathWay		m_Target;
		int			m_PathId{ 0 };
		MathLib::FEnemyStats m_Stats{};
		GameObject*			 m_Target{ nullptr };
	};
} // namespace dae