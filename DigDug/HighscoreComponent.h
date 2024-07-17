#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"
#include "InputManager.h"
#include <SDL.h>

namespace dae {
    class HighscoreComponent final : public Component

    {
    public:
        HighscoreComponent(Scene* scene, int scoreNr, std::function<void()> createHighscoreScreen) : m_Scene{ scene }, m_Score{ scoreNr },m_CreateHighscoreScreen { createHighscoreScreen } { };
        ~HighscoreComponent() = default;
        HighscoreComponent(const HighscoreComponent&) = delete;
        HighscoreComponent(HighscoreComponent&&) noexcept = delete;
        HighscoreComponent& operator=(const HighscoreComponent&) = delete;
        HighscoreComponent& operator=(HighscoreComponent&&) noexcept = delete;
        virtual void Update() override { m_ControlTimer -= Timer::GetInstance().GetDeltaTime(); };
        virtual void Render() const override;
        virtual void Init() override;

        void MoveCursor(glm::vec2 key);
        void Select();

    private:
        Scene* m_Scene{ nullptr };
        std::vector<std::string> m_KeyboardKeys{ "a","z","e","r","t","y","u","i","o","p","q","s","d","f","g","h","j","k","l","m","w","x","c","v","b","n" };
        std::vector<SDL_Rect> m_KeyboardKeyRects{};

        void MakePlayerInput(Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow);
        void MapControllerInput(int id, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow);
        void MapKeyboardInput(int id, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow);

        int m_SelectedKey{ 0 };
        std::string m_Name{ "Enter Name" };
        std::function<void()> m_CreateHighscoreScreen;
        int m_Score{ 0 };
        float m_ControlTimer{ 0.2f };
    };
}
