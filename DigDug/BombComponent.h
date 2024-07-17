#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {
    class BombComponent final : public Component

    {
    public:
        BombComponent(Scene* scene) : m_Scene{ scene } { };
        ~BombComponent() = default;
        BombComponent(const BombComponent&) = delete;
        BombComponent(BombComponent&&) noexcept = delete;
        BombComponent& operator=(const BombComponent&) = delete;
        BombComponent& operator=(BombComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;

        void StartBomb(glm::vec3 loc);

    private:
        Scene* m_Scene{ nullptr };
        float m_DestroyTimer{ 0.7f };
    };
}
