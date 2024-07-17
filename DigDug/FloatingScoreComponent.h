#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {
    class FloatingScoreComponent final : public Component

    {
    public:
        FloatingScoreComponent(Scene* scene, int score, glm::vec3 position) : m_Scene{ scene }, m_Score{ score }, m_Position{ position } { };
        ~FloatingScoreComponent() = default;
        FloatingScoreComponent(const FloatingScoreComponent&) = delete;
        FloatingScoreComponent(FloatingScoreComponent&&) noexcept = delete;
        FloatingScoreComponent& operator=(const FloatingScoreComponent&) = delete;
        FloatingScoreComponent& operator=(FloatingScoreComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override {};
        virtual void Init() override;


    private:
        Scene* m_Scene{ nullptr };
        int m_Score{ 0 };
        glm::vec3 m_Position{};
        float m_Speed{ 20 }, m_Timer{ 2 };
    };
}
