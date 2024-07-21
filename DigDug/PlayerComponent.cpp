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
	SetState(new AliveState(m_Scene), MathLib::ELifeState::ALIVE);
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
	GetGameObject()->GetTransform()->SetPosition(m_OriginalPosition);
}

void dae::PlayerComponent::ActivateUnderlyingThing(PathWay& pathway)
{
	if (pathway.PathStats.PathState == MathLib::EPathState::Powerup) {
		switch (pathway.PathStats.PowerupType)
		{
		case MathLib::EPowerupType::ExtraBomb:
			m_MaxBombs++;
			break;
		case MathLib::EPowerupType::Detonator:
			break;
		case MathLib::EPowerupType::Flames:
			m_BombStrength++;
			break;
		case MathLib::EPowerupType::None:
			break;
		default:
			break;
		}

		pathway.PathStats.PathType = MathLib::EPathType::Tile;
		pathway.PathStats.PathState = MathLib::EPathState::Tile;
		pathway.PathStats.HasUnderlyingThing = false;
		pathway.PathStats.PowerupType = MathLib::EPowerupType::None;
		pathway.TextureComponent->SetIsVisible(false);
	}
	else {
		Event scoreEvent{ EventType::DoorTrigger };
		Notify(GetGameObject(), scoreEvent);
	}
}
