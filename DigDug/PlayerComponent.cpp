#include "PlayerComponent.h"
#include <ValuesComponent.h>
#include "MenuComponent.h"
#include "EntityMovementComponent.h"
#include <InputComponent.h>
#include "EnemyComponent.h"

dae::PlayerComponent::~PlayerComponent()
{
	m_PlayerState.reset();
}

void dae::PlayerComponent::Init()
{
	SetState(new AliveState(m_Scene), MathLib::ALIVE);
}

void dae::PlayerComponent::Update()
{
	m_PlayerState->Update(GetGameObject());
}

void dae::PlayerComponent::Render() const
{
}

void dae::PlayerComponent::Reposition()
{
	GetGameObject()->GetTransform()->Translate(m_OriginalPosition);
}