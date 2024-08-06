#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "EntityMovementComponent.h"
#include "MenuComponent.h"
#include <InputComponent.h>
#include <ValuesComponent.h>
#include <Renderer.h>

dae::PlayerComponent::~PlayerComponent() { m_PlayerState.reset(); }

void dae::PlayerComponent::Init() { SetState(new AliveState(m_Scene), MathLib::ELifeState::ALIVE); }

void dae::PlayerComponent::Update() { m_PlayerState->Update(GetGameObject()); }

void dae::PlayerComponent::Render() const {

	 SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	 SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &GetGameObject()->GetComponent<TextureComponent>()->GetRect()); // D
}

void dae::PlayerComponent::Reposition() { GetGameObject()->GetTransform()->SetPosition(m_OriginalPosition); }

void dae::PlayerComponent::ActivateUnderlyingThing(PathWay& pathway)
{
	if (pathway.PathStats.PathState == MathLib::EPathState::Powerup)
	{
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
		pathway.PathObject->GetComponent<TextureComponent>()->SetIsVisible(false);
	}
	else
	{
		Event scoreEvent{ EventType::DoorTrigger };
		Notify(GetGameObject(), scoreEvent);
	}
}
