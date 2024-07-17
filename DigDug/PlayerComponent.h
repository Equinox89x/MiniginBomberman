#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {
    class PlayerState {
    public:
        virtual ~PlayerState() = default;
        virtual void Update() = 0;
        virtual void Init() = 0;

        void SetData(dae::Scene* scene, GameObject* go) {
            if (scene) {
                m_Scene = scene;
            }
            if (go) {
                gameObject = go;
            }
        }

        void ClearGameObject() { gameObject = nullptr; }

    protected:
        Scene* m_Scene{ nullptr };
        GameObject* gameObject{ nullptr };
    };

    class RespawnState : public PlayerState {
    public:
        virtual void Init() override;
        virtual void Update() override;

    private:
        float m_Timer{ 3 };
    };    
    
    class DeadState : public PlayerState {
    public:
        virtual void Init() override {};
        virtual void Update() override{};

    private:
    };

    class AliveState : public PlayerState {
    public:
        virtual void Init() override {};
        virtual void Update() override;
    private:
        int* m_Ptr = reinterpret_cast<int*>(0xdddddddddddddddd);

    };

    class InvincibleState : public PlayerState {
    public:
        virtual void Init() override {};
        virtual void Update() override;
    private:
        float m_Timer{ 3 };

    };

    class PlayerComponent final : public Component
    {
    public:
        PlayerComponent(Scene* scene, glm::vec2 startPos) : m_Scene{ scene }, m_OriginalPosition{ startPos } { };
        ~PlayerComponent();
        PlayerComponent(const PlayerComponent&) = delete;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent& operator=(const PlayerComponent&) = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;

        void Reposition();

        MathLib::ELifeState GetState() { return m_State; };
        void Respawn();
        void SetState(PlayerState* state, MathLib::ELifeState lifeState) {
            delete m_PlayerState;
            m_State = lifeState;
            m_PlayerState = state;
            auto go{ GetGameObject() };
            m_PlayerState->SetData(m_Scene, go);
            m_PlayerState->Init();
        }
        PlayerState* GetPlayerState() { return m_PlayerState; };


    private:
        Scene* m_Scene{ nullptr };
        PlayerState* m_PlayerState{ nullptr };

        MathLib::ELifeState m_State{ MathLib::ELifeState::ALIVE };
        glm::vec2 m_OriginalPosition;
        float m_InvincibilityTimer{ 3 };
    };
}

