#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include <Scene.h>
#include <MathLib.h>
#include <functional>

namespace dae {
    class MenuComponent final : public Component
    {
    public:
        MenuComponent(Scene* scene, std::function<void(MathLib::GameMode gm, int levelIndex)> createLevel, std::function<void()> createHighscoreScreen, std::vector<glm::vec2> cursorLocations, MathLib::GameMode gamemode = MathLib::GameMode::SOLO) :
            m_Scene{ scene }, CreateLevel{ createLevel }, CreateHighscoreMenu{ createHighscoreScreen }, m_CursorLocations{ cursorLocations }, m_SelectedGameMode{gamemode} {  };
        ~MenuComponent() = default;

        MenuComponent(const MenuComponent&) = delete;
        MenuComponent(MenuComponent&&) noexcept = delete;
        MenuComponent& operator=(const MenuComponent&) = delete;
        MenuComponent& operator=(MenuComponent&&) noexcept = delete;

        void CycleGameMode(bool isMoveUp);
        void StartGame(GameObject* pMenu);
        void SkipLevel();
        void GameOver();

        void Update() override;

    private:
        Scene* m_Scene{ nullptr };


        MathLib::GameMode m_SelectedGameMode{ MathLib::GameMode::SOLO };
        bool m_IsMainMenu{ true };
        bool m_IsMenu{ false };
        std::vector<glm::vec2> m_CursorLocations{};

        std::function<void(MathLib::GameMode gm, int levelIndex)> CreateLevel;
        std::function<void()> CreateHighscoreMenu;
        float m_ControlTimer{ 0.2f };
    };
}
