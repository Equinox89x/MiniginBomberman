#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"
#include "States.h"

namespace dae {
    /*class EntityState {
    public:
        virtual ~EntityState() = default;
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

    class RespawnState : public EntityState {
    public:
        virtual void Init() override;
        virtual void Update() override;

    private:
        float m_Timer{ 3 };
    };    
    
    class DeadState : public EntityState {
    public:
        virtual void Init() override {};
        virtual void Update() override{};

    private:
    };

    class AliveState : public EntityState {
    public:
        virtual void Init() override {};
        virtual void Update() override;
    private:
        int* m_Ptr = reinterpret_cast<int*>(0xdddddddddddddddd);

    };

    class InvincibleState : public EntityState {
    public:
        virtual void Init() override {};
        virtual void Update() override;
    private:
        float m_Timer{ 3 };

    };*/

    class PlayerComponent final : public Component
    {
    public:
        PlayerComponent(Scene* scene, glm::vec2 startPos) : m_Scene{ scene }, m_OriginalPosition{ startPos } { };
        ~PlayerComponent();
        PlayerComponent(const PlayerComponent&) = delete;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent& operator=(const PlayerComponent&) = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;
        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() const override;

        void Reposition();

        void SetState(EntityState* playerState, MathLib::ELifeState lifeState) {
            if (playerState)
            {
                if(m_PlayerState) m_PlayerState->OnEnd(GetGameObject());
                m_PlayerState = std::unique_ptr<EntityState>(playerState);
                m_State = lifeState;
                if(m_PlayerState) m_PlayerState->OnStart(GetGameObject());
            }
        }
        MathLib::ELifeState GetState() { return m_State; };
        EntityState* GetPlayerState() { return m_PlayerState.get(); };


    private:
        Scene* m_Scene{ nullptr };
        std::unique_ptr<EntityState> m_PlayerState{ nullptr };
        MathLib::ELifeState m_State{ MathLib::ELifeState::ALIVE };

        glm::vec2 m_OriginalPosition;
    };
}

