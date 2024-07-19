#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"
#include "PathwayCreatorComponent.h"


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

        void StartBomb(glm::vec3 loc, int tileId);


    private:
        Scene* m_Scene{ nullptr };
        float m_FuseTimer{ 1.2f }, m_ExplodeTimer{ 1.3f+ m_FuseTimer };
        int m_BombStrength{ 2 };
        int m_TileId{ 0 };
        bool m_HitWallLeft{ false }, m_HitWallRight{ false }, m_HitWallTop{ false }, m_HitWallBottom{ false };

        void HandleExplosionEnd(int& leftIndex);
        void HandleExplosionPlacement(int& leftIndex, const std::map<int, dae::PathWay>& pathways, bool& outHitWall);
    };
}
