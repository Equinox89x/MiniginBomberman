#include "EnemyComponent.h"
#include "EntityMovementComponent.h"
//#include "FloatingScoreComponent.h"
#include "GameObject.h"
#include "Observers.h"
#include "PlayerComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include <AudioComponent.h>
#include <TextObjectComponent.h>
#include <ValuesComponent.h>

dae::EnemyComponent::~EnemyComponent()
{
	m_EnemyState.reset();

	m_Player = nullptr;
}

void dae::EnemyComponent::Init() { SetState(new AliveState(m_Scene), MathLib::ELifeState::ALIVE); }

void dae::EnemyComponent::Update() { m_EnemyState->Update(GetGameObject()); }

void dae::EnemyComponent::Render() const
{
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect0);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255);
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect1);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect2);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Scoreect3);
}