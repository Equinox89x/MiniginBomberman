#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "MathLib.h"
#include "Scene.h"
#include "Subject.h"
#include "States.h"

namespace {
    constexpr SDL_Rect m_ScoreRect0{ 0, 45 * 3, 1000, int(45 * 1.5f) };
    constexpr SDL_Rect m_ScoreRect1{ 0, int(45 * 4.5), 1000, int(45 * 1.75f) };
    constexpr SDL_Rect m_ScoreRect2{ 0, int(45 * 6.25), 1000, int(45 * 2) };
    constexpr SDL_Rect m_ScoreRect3{ 0,int(45 * 8.25), 1000, int(45 * 2) };
}

namespace dae {

    enum class EEnemyType {
        Pooka,
        Fygar
    };

    class EnemyComponent final : public Component, public Subject
    {
    public:
        EnemyComponent(Scene* scene, EEnemyType enemyType = EEnemyType::Pooka, bool isVersus = false) : m_Scene{ scene }, m_EnemyType{ enemyType }, m_IsVersus{ isVersus } {  };
        ~EnemyComponent();
        EnemyComponent(const EnemyComponent&) = delete;
        EnemyComponent(EnemyComponent&&) noexcept = delete;
        EnemyComponent& operator=(const EnemyComponent&) = delete;
        EnemyComponent& operator=(EnemyComponent&&) noexcept = delete;
        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() const override;

        void SetState(EntityState* playerState, MathLib::ELifeState lifeState) {
            if (playerState)
            {
                if(m_EnemyState) m_EnemyState->OnEnd(GetGameObject());
                m_EnemyState = std::unique_ptr<EntityState>(playerState);
                m_State = lifeState;
                if (m_EnemyState) m_EnemyState->OnStart(GetGameObject());
            }
        }
        MathLib::ELifeState GetState() { return m_State; };
        EntityState* GetEnemyState() { return m_EnemyState.get(); };
        EEnemyType GetEnemyType() { return m_EnemyType; };

        void SetPlayer(GameObject* player) {
            m_Player = player;
        }
        GameObject* GetPlayer() { return m_Player; };
        
    private:
        Scene* m_Scene{ nullptr };
        std::unique_ptr<EntityState> m_EnemyState{ nullptr };
        MathLib::ELifeState m_State{ MathLib::ELifeState::ALIVE };

        int currentPumpStage{ 0 }, maxPumpStage{ 4 };
        MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };
        bool m_IsVersus{ false };

        EEnemyType m_EnemyType{ EEnemyType::Pooka };
        GameObject* m_Player{ nullptr };


    };
}
