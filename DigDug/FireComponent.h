#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {
    class FireComponent  final : public Component
    {
    public:
        FireComponent(Scene* scene, GameObject* gameObject) : m_Scene{ scene }, m_EnemyGameObject{ gameObject }, m_OriginLocation { gameObject->GetTransform()->GetWorldPosition() } { };
        ~FireComponent() = default;
        FireComponent(const FireComponent&) = delete;
        FireComponent(FireComponent&&) noexcept = delete;
        FireComponent& operator=(const FireComponent&) = delete;
        FireComponent& operator=(FireComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override {};
        virtual void Init() override;

        bool GetIsFireFinished() { return m_FireFinished; };

    private:
        Scene* m_Scene{ nullptr };
        float m_Timer{ 0.8f };
        bool m_FireFinished{ false };
        glm::vec2 m_OriginLocation{};
        GameObject* m_EnemyGameObject{ nullptr };
        std::string m_Direction{ "Left" };
    };

}

