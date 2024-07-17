#include "FPSCounterComponent.h"
#include "TextObjectComponent.h"
#include "TransformComponent.h"
#include "Timer.h"
#include <SDL_ttf.h>
#include <stdexcept>
#include <Windows.h>
#include <memory>

dae::FPSCounterComponent::FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font) :
	TextObjectComponent(text, font)
{ }

void dae::FPSCounterComponent::Update()
{
	TextObjectComponent::Update();

	TextObjectComponent::SetText(std::to_string(Timer::GetInstance().GetFPS()) + " FPS");
}

void dae::FPSCounterComponent::Render() const
{
	TextObjectComponent::Render();
}

void dae::FPSCounterComponent::SetPosition(const float x, const float y)
{
	TextObjectComponent::SetPosition(x, y);
}


