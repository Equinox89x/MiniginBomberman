#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {
    class PumpComponent final : public Component

    {
    public:
        PumpComponent(Scene* scene) : m_Scene{ scene } { };
        ~PumpComponent() = default;
        PumpComponent(const PumpComponent&) = delete;
        PumpComponent(PumpComponent&&) noexcept = delete;
        PumpComponent& operator=(const PumpComponent&) = delete;
        PumpComponent& operator=(PumpComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;

        void Pump(glm::vec3 loc);
        void ResetMovement(glm::vec3 loc);
        void ReleasePump();
        void AttachPump(GameObject* enemy);
        void SetPlayer(GameObject* player);


    private:
        Scene* m_Scene{ nullptr };
        glm::vec2 m_StartPos{};
        bool m_CanMove{ false };
        GameObject* m_SelectedEnemy{nullptr};
        GameObject* m_Player{nullptr};
        float m_DestroyTimer{ 0.7f };
        void UpdatePosition();
    };
}
