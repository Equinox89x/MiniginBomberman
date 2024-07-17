#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "PathwayCreatorComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include <map>
#include "Scene.h"


namespace dae {
    class EntityMovementComponent final : public Component
    {

    public:
        EntityMovementComponent(Scene* scene, glm::vec2 startPos, bool isAutonomous = false) : m_Scene{ scene }, m_StartPos{ startPos }, m_IsAutonomous{ isAutonomous } { };
        ~EntityMovementComponent() = default;
        EntityMovementComponent(const EntityMovementComponent&) = delete;
        EntityMovementComponent(EntityMovementComponent&&) noexcept = delete;
        EntityMovementComponent& operator=(const EntityMovementComponent&) = delete;
        EntityMovementComponent& operator=(EntityMovementComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override { };

        void SetIsController(bool isController) { m_IsController = isController; };

        void SetStartPos(glm::vec2 startPos) { m_StartPos = startPos; };
        void Reposition() { GetGameObject()->GetComponent<TextureComponent>()->SetPosition(m_StartPos.x, m_StartPos.y); };

        void SetMovement(MathLib::Movement movement);

        MathLib::ELifeState GetState() { return m_PlayerState; };
        SDL_Rect GetCollider() { return m_PathwayColliderRect; };
        SDL_Rect GetCharacterCollider() { return m_Rect; };
        bool GetShouldDig() { return m_ShouldDig; };
        void SetShouldDig(bool shouldDig) { m_ShouldDig = shouldDig; };

        void SetNextTileId(int id) { m_NextTileId = id; };
        void SetCurrentTileId(int id);        
        int GetNextTileId() { return m_NextTileId; };
        int GetCurrentTileId() { return m_CurrentTileId; };

        SDL_Rect GetPathCollider(MathLib::Movement movement);

        void SetMapBorders(SDL_Rect LeftMapBorder, SDL_Rect RightMapBorder, SDL_Rect TopMapBorder, SDL_Rect BottomMapBorder) {
            m_LeftMapBorder = LeftMapBorder;
            m_RightMapBorder = RightMapBorder;
            m_TopMapBorder = TopMapBorder;
            m_BottomMapBorder = BottomMapBorder;
        };

        void DisableMovement(bool isDisabled) { m_CanMove = !isDisabled; };
        MathLib::Movement GetDirection() { return m_Movement; };
        std::string GetLastDirection() { return m_LastDir; }
        void SetLastDirection(std::string dir) { m_LastDir = dir; }

        void SetEnemyName(std::string enemyName) {
            m_EnemyName = enemyName;
        }
        std::string GetEnemyName() { return m_EnemyName; };

        void SetGhostModeEnabled(bool isEnabled) { m_IsGhostMode = isEnabled; };
        void SetGhostLocation(glm::vec2 loc) { m_CachedLocation = loc; };

    private:
        bool m_IsController{ false }, m_ShouldDig{ false }, m_IsAutonomous{ false }, m_CanMove{ true };
        int m_NextTileId{ 0 }, m_CurrentTileId{ 0 };
        std::map<MathLib::Movement, int> m_CurrentSurroundingTiles{};
        Scene* m_Scene{ nullptr };

        SDL_Rect m_BottomRect{}, m_TopRect{}, m_Rect{}, m_LeftRect{}, m_RightRect{}, m_PathwayColliderRect{};
            
        MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };
        MathLib::Movement m_Movement{ MathLib::Movement::DOWN };
        MathLib::Movement m_PrevMovement{};
        glm::vec2 m_StartPos{};
        glm::vec2 m_PrevLoc{};

        void CheckMovement(const std::map<int, PathWay>& pathways);

        //Autonomous
        float m_Speed{ 50 }, m_MoveTimer{ 2 };
        bool m_CanChangeState{ false }, m_IsGhostMode{ false };
        SDL_Rect m_LeftMapBorder{}, m_RightMapBorder{}, m_TopMapBorder{}, m_BottomMapBorder{};
        MathLib::EMovingState m_State{ MathLib::EMovingState::MovingLeft };
        std::map<MathLib::EMovingState, bool> m_MovementDir{
              std::make_pair(MathLib::EMovingState::MovingUp, true),
              std::make_pair(MathLib::EMovingState::MovingLeft, true),
              std::make_pair(MathLib::EMovingState::MovingRight, true),
              std::make_pair(MathLib::EMovingState::MovingDown, true),
        };
        std::string m_EnemyName{"Pooka"};
        std::string m_LastDir{"Left"};
        glm::vec2 m_CachedLocation{0,0};
        PathWay m_Target;
    };
}