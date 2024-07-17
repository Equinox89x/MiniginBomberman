#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "Scene.h"
#include "PathwayCreatorComponent.h"
#include "Subject.h"

namespace dae {
    class RockComponent final : public Component, public Subject

    {
    public:
        RockComponent(Scene* scene) : m_Scene{ scene } { };
        ~RockComponent() = default;
        RockComponent(const RockComponent&) = delete;
        RockComponent(RockComponent&&) noexcept = delete;
        RockComponent& operator=(const RockComponent&) = delete;
        RockComponent& operator=(RockComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override {};
        virtual void Init() override {};

        int GetPlayerId() { return m_PlayerId; }
        int GetScore() { return m_Scores[m_EnemiesHit]; }

    private:
        Scene* m_Scene{ nullptr };
        int m_Score{ 0 };
        float m_Speed{ 150 }, m_MoveTimer{ 0.2f }, m_TimeBeforeFall{ 0.5f }, m_DeathTimer{0.3f};
        bool m_PlayerHasPassed{ false }, m_Skipcheck{ false };

        std::map<int, int> m_Scores{ {0, 0}, { 1, 1000 }, { 2,2500 }, { 3,4000 }, { 4,6000 }, { 5,8000 }, { 6,10000 }, { 7,12000 }, { 8,15000 }};
        int m_PlayerId, m_EnemiesHit{ 0 };
    };
}
