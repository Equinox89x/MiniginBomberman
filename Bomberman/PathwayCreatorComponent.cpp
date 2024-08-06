#include "PathwayCreatorComponent.h"
#include "BombComponent.h"
#include "EnemyComponent.h"
#include "EntityMovementComponent.h"
#include "GameObject.h"
#include "Observers.h"
#include "PlayerComponent.h"
#include "Renderer.h"
#include <ValuesComponent.h>
#include <iostream>
#include <memory>

dae::PathwayCreatorComponent::~PathwayCreatorComponent()
{
	for (auto pathway : m_Pathways)
	{
		pathway.second.PathObject->Cleanup();
		pathway.second.PathObject = nullptr;
	}
	m_Pathways.clear();
}

void dae::PathwayCreatorComponent::AddPathway(int id, glm::vec2 pos, std::string type)
{
	GameObject* go = new GameObject();
	// m_pScene->Add(go);
	go->SetName(std::to_string(id));

	glm::vec2 pos2{ pos.x, pos.y };
	go->GetTransform()->SetPosition(pos2);

	auto comp{ std::make_unique<TextureComponent>() };
	comp->SetName(std::to_string(id));
	comp->SetTexture("Levels/tiles.png", 0.1f, 3, false, false); 
	comp->SetFrame(static_cast<int>(m_PathStats[type].PathType));
	go->AddComponent(std::move(comp));

	if (m_PathStats[type].PathState == MathLib::EPathState::Powerup)
	{
		m_PathStats[type].HasUnderlyingThing = true;
		auto value{ MathLib::CalculateChance(90) };
		if (value >= 0 && value <= 30)
		{
			m_PathStats[type].PowerupType = MathLib::EPowerupType::ExtraBomb;
		}
		if (value > 30 && value <= 60)
		{
			m_PathStats[type].PowerupType = MathLib::EPowerupType::Flames;
		}
		if (value > 60 && value <= 90)
		{
			m_PathStats[type].PowerupType = MathLib::EPowerupType::Detonator;
		}
	}

	auto go2 = GetGameObject()->AddChild(std::move(go));
	SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(Cellsize), static_cast<int>(Cellsize) };
	PathWay	 pathWay{ id, go2, m_PathStats[type], { rect.x + rect.w / 2, rect.y + rect.h / 2 }, rect
	};

	if (type == "spawn")
	{
		m_Spawns.push_back(pathWay);
	}
	else if (type == "enemySpawn")
	{
		m_EnemySpawns.push_back(pathWay);
	}

	m_Pathways.insert({ id, pathWay });
}

void dae::PathwayCreatorComponent::ActivatePathway(int id)
{
	if (m_Pathways.find(id) != m_Pathways.end())
	{
		auto& path{ m_Pathways[id] };
		if (path.PathStats.PathType != MathLib::EPathType::Blocker)
		{

			path.PathStats.PathType = MathLib::EPathType::Tile;
			if (path.PathStats.HasUnderlyingThing)
			{
				path.PathObject->GetComponent<TextureComponent>()->SetTexture("Levels/powerups.png", 0.1f, 4, false, false);
				path.PathObject->GetComponent<TextureComponent>()->SetFrame(static_cast<int>(path.PathStats.PowerupType));
			}
			else
			{
				path.PathStats.PathState = MathLib::EPathState::Tile;
				path.PathObject->GetComponent<TextureComponent>()->SetTexture("Levels/tiles.png", 0.1f, 3, false, false);
				path.PathObject->GetComponent<TextureComponent>()->SetFrame(static_cast<int>(path.PathStats.PathType));

			}
		}
	}
}

void dae::PathwayCreatorComponent::ActivateBomb(int id)
{
	if (m_Pathways.find(id) != m_Pathways.end())
	{
		auto& path{ m_Pathways[id] };
		path.PathObject->GetComponent<TextureComponent>()->SetIsVisible(true);
		path.PathStats.PathType = MathLib::EPathType::Tile;
		path.PathStats.PathState = MathLib::EPathState::Explosion;
	}
}

void dae::PathwayCreatorComponent::PickDoorFromBreakableTiles()
{
	std::vector<int> breakablePathwayIds{};
	for (auto& pathWay : m_Pathways)
	{
		if (pathWay.second.PathStats.PathState == MathLib::EPathState::Breakable)
		{
			breakablePathwayIds.push_back(pathWay.first);
		}
	}

	int	  randomId{ MathLib::CalculateChance(static_cast<int>(breakablePathwayIds.size()) - 1) };
	auto& path{ m_Pathways.at(breakablePathwayIds[randomId]) };
	path.PathStats.PathState = MathLib::EPathState::Door;
	path.PathStats.HasUnderlyingThing = true;
	path.PathStats.PowerupType = MathLib::EPowerupType::Door;
}

void dae::PathwayCreatorComponent::Update()
{
	for (auto& path : m_Pathways)
	{
		auto	 pos = path.second.PathObject->GetTransform()->GetWorld().Position;
		SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(Cellsize), static_cast<int>(Cellsize) };
		path.second.Rect = rect;
		path.second.Middle = { pos.x + rect.w / 2, pos.y + rect.h / 2 };
	}
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::HandleEntityTileOverlap()
{
	auto& children{ GetGameObject()->GetChildren() };
	if (children.empty())
		return;
	if (!m_pScene)
		return;
	m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
	m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);

	if (!m_pCharacters.empty())
	{
		for (GameObject* character : m_pCharacters)
		{
			if (children.empty())
				return;
			if (character->IsMarkedForDestroy())
				continue;
			if (const auto moveComp{ character->GetComponent<EntityMovementComponent>() })
			{
				for (GameObject* path : GetGameObject()->GetChildren())
				{
					if (!children.empty() && path)
					{
						HandleTileChange(character, path, moveComp);
					}
				}
			};
		}
	}

	if (!m_pEnemies.empty())
	{
		for (GameObject* enemy : m_pEnemies)
		{
			if (children.empty())
				return;
			if (enemy->IsMarkedForDestroy())
				continue;
			if (const auto moveComp{ enemy->GetComponent<EntityMovementComponent>() })
			{
				for (GameObject* path : GetGameObject()->GetChildren())
				{
					if (!children.empty() && path)
					{
						HandleTileChange(enemy, path, moveComp);
					}
				}
			};
		}
	}
}

void dae::PathwayCreatorComponent::HandleTileChange(GameObject* entity, GameObject* path, EntityMovementComponent* moveComp)
{
	if (!path || path->IsMarkedForDestroy())
		return;
	auto texComp{ path->GetComponent<TextureComponent>() };
	if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCollider()))
	{
		moveComp->SetNextTileId(std::stoi(path->GetName()));
	}
	if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCharacterCollider()))
	{
		moveComp->SetCurrentTileId(std::stoi(path->GetName()));
	}

	auto& pathway{ m_Pathways.at(moveComp->GetCurrentTileId()) };
	if (pathway.PathStats.PathState == MathLib::EPathState::Explosion)
	{
		if (auto enemyComp{ entity->GetComponent<EnemyComponent>() })
		{
			if (enemyComp->GetState() == MathLib::ELifeState::ALIVE)
			{
				enemyComp->SetPlayer(pathway.BombDropper);
				enemyComp->SetState(new BombedState(m_pScene), MathLib::ELifeState::BOMBED);
			}
		}
		if (auto playerComp{ entity->GetComponent<PlayerComponent>() })
		{
			if (playerComp->GetState() == MathLib::ELifeState::ALIVE)
			{
				playerComp->SetState(new BombedState(m_pScene), MathLib::ELifeState::BOMBED);
			}
		}
	}
	else if (pathway.PathStats.HasUnderlyingThing)
	{
		if (auto playerComp{ entity->GetComponent<PlayerComponent>() })
		{
			if (playerComp->GetState() == MathLib::ELifeState::ALIVE)
			{
				playerComp->ActivateUnderlyingThing(pathway);
			}
		}
	}
}

void dae::PathwayCreatorComponent::Init()
{
	m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
	m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);

	m_PathStats = std::map<std::string, MathLib::EPathStats>{
		{ "tile", MathLib::EPathStats{ MathLib::EPathState::Tile, MathLib::EPathType::Tile, false } },
		{ "spawn", MathLib::EPathStats{ MathLib::EPathState::Spawn, MathLib::EPathType::Tile, false } },
		{ "enemySpawn", MathLib::EPathStats{ MathLib::EPathState::EnemySpawn, MathLib::EPathType::Tile, false } },
		{ "blocker", MathLib::EPathStats{ MathLib::EPathState::Blocker, MathLib::EPathType::Blocker, false } },
		{ "breakable", MathLib::EPathStats{ MathLib::EPathState::Breakable, MathLib::EPathType::Breakable, false } },
		{ "powerup", MathLib::EPathStats{ MathLib::EPathState::Powerup, MathLib::EPathType::Breakable, true } },
	};
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::Render() const
{
	for (const auto& path : m_Pathways)
	{
		auto rrect = path.second.Rect;
		if (path.second.PathStats.PathType == MathLib::EPathType::Blocker)
		{
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
		}
		else if (path.second.PathStats.PathType == MathLib::EPathType::Tile)
		{
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 49, 154, 0, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 200);
		}
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);
	}
}
