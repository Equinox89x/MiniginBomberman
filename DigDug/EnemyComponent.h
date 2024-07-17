#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "MathLib.h"
#include "Scene.h"
#include "Subject.h"

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

    class EnemyState {
    public:
        virtual ~EnemyState() = default;
        virtual void Update() = 0;
        virtual void Init() = 0;

        void SetData(dae::Scene* scene, GameObject* go, EEnemyType enemyType, bool isVersus) {
            m_Scene = scene;
            gameObject = go;
            EnemyType = enemyType;
            m_IsVersus = isVersus;
        }

    protected:
        Scene* m_Scene{ nullptr };
        GameObject* gameObject{ nullptr };
        MathLib::Movement m_Movement{ MathLib::Movement::DOWN };
        EEnemyType EnemyType{ EEnemyType::Pooka };
        bool m_IsVersus{ false };

    };

    class MovingState : public EnemyState {
    public:
        virtual void Init() override;
        virtual void Update() override;

    private:
        float m_Timer{ 5 };
    };   
    
    class InflatingState : public EnemyState {
    public:
        virtual void Init() override {};
        virtual void Update() override;
    };
        
    class GhostState : public EnemyState {
    public:
        virtual void Init() override;
        virtual void Update() override;
    private:
        glm::vec2 m_CachedLocation{};
    };       

    class BreatheFireState : public EnemyState {
    public:
        virtual void Init() override;
        virtual void Update() override;
    private:
        float m_FireTimer{ 0.3f }, m_PrepareTimer{ 0.6f };
        GameObject* fireObject{ nullptr };
        bool m_IsPrepareComplete{ false };
    };

    class DeathState : public EnemyState, public Subject {
    public:
        virtual void Init() override;
        virtual void Update() override;
    private:
        float m_DeathTimer{ 0.3f };
        int m_Score{ 200 };
    };


    //MovingState* EnemyState::moving = new MovingState();
    //InflatingState* EnemyState::pumping = new InflatingState();
    //GhostState* EnemyState::ghosting = new GhostState();
    //DeathState* EnemyState::death = new DeathState();


    class EnemyComponent final : public Component, public Subject
    {
    public:
        EnemyComponent(Scene* scene, EEnemyType enemyType = EEnemyType::Pooka, bool isVersus = false) : m_Scene{ scene }, EnemyType{ enemyType }, m_IsVersus{ isVersus } { SetState(new MovingState); };
        ~EnemyComponent();
        EnemyComponent(const EnemyComponent&) = delete;
        EnemyComponent(EnemyComponent&&) noexcept = delete;
        EnemyComponent& operator=(const EnemyComponent&) = delete;
        EnemyComponent& operator=(EnemyComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;
        MathLib::ELifeState GetLifeState() { return m_PlayerState; };
        void SetLifeState(MathLib::ELifeState state) { m_PlayerState = state; };

        bool PumpUp();

        void SetState(EnemyState* state) {
            delete m_State;
            m_State = state;
            m_State->SetData(m_Scene, GetGameObject(), EnemyType, m_IsVersus);
            m_State->Init();
        }
        EnemyState* GetState() { return m_State; };

        void SetPlayer(GameObject* player) {
            m_Player = player;
        }
        GameObject* GetPlayer() { return m_Player; };
        
    private:
        Scene* m_Scene{ nullptr };
        int currentPumpStage{ 0 }, maxPumpStage{ 4 };
        MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };
        bool m_IsVersus{ false };

        EnemyState* m_State{ nullptr };
        EEnemyType EnemyType{ EEnemyType::Pooka };
        GameObject* m_Player{ nullptr };


    };
}
