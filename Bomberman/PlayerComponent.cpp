#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "EntityMovementComponent.h"
#include "MenuComponent.h"
#include <InputComponent.h>
#include <ValuesComponent.h>
#include <Renderer.h>
#include "BombComponent.h"

dae::PlayerComponent::~PlayerComponent() { m_PlayerState.reset(); }

void dae::PlayerComponent::Init() { SetState(new AliveState(m_Scene), MathLib::ELifeState::ALIVE); }

void dae::PlayerComponent::Update() { m_PlayerState->Update(GetGameObject()); }
//
//void dae::PlayerComponent::Render() const {
//
//	 SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
//	 SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &GetGameObject()->GetComponent<TextureComponent>()->GetRect()); // D
//}

void dae::PlayerComponent::Reposition()
{
	 GetGameObject()->GetComponent<InputComponent>()->ResetOverlap(m_OriginalPosition);
	 GetGameObject()->GetTransform()->SetPosition(m_OriginalPosition);
}

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
			HasDetonator = true;
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

dae::GameObject* dae::PlayerComponent::GetOldestBomb(std::string playerName) { 
	auto bombs{ m_Scene->GetGameObjects(EnumStrings[Names::Bomb]) };
	std::vector<GameObject*> playerBombs{};
		std::copy_if(
		  bombs.begin(), bombs.end(), std::back_inserter(playerBombs), [&](GameObject * go) {
			  auto bombComp{ go->GetComponent<BombComponent>() };
					  return bombComp->BombDropper->GetName() == playerName;
		});

	float oldestBombValue{ 0 };
	GameObject* oldestBomb{ nullptr };
	for (auto bomb : playerBombs)
	{
		if (bomb->GetComponent<BombComponent>()->LifeTime > oldestBombValue)
		{
			oldestBombValue = bomb->GetComponent<BombComponent>()->LifeTime;
			oldestBomb = bomb;
		}
	}

	return oldestBomb;
}
