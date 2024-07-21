#include "PathwayCreatorComponent.h"
#include "GameObject.h"
#include "EntityMovementComponent.h"
#include "Renderer.h"
#include <memory>
#include <iostream>
#include "EnemyComponent.h"
#include "PlayerComponent.h"
#include <ValuesComponent.h>
#include "BombComponent.h"

dae::PathwayCreatorComponent::~PathwayCreatorComponent()
{
	m_Pathways.clear();
}

void dae::PathwayCreatorComponent::AddPathway(int id, glm::vec2 pos, std::string type)
{
	auto go = std::make_shared<GameObject>();
	m_pScene->Add(go);
	GetGameObject()->AddChild(go.get());
	go->SetName(std::to_string(id));

	glm::vec2 pos2{ pos.x, pos.y };
	go->GetTransform()->SetPosition(pos2);

	auto comp{ std::make_unique<TextureComponent>() };
	comp->SetName(std::to_string(id));
	comp->SetTexture("Levels/" + m_PathStats[type].TextureName + ".png", 0.1f, 1, false, false);
	auto component{ go->AddComponent(std::move(comp)) };
	//component->SetPosition(pos.x, pos.y);
	if (m_PathStats[type].PathType == MathLib::EPathType::Tile) {
		component->SetIsVisible(false);
	}

	if (m_PathStats[type].PathState == MathLib::EPathState::Powerup) {
		auto value{ MathLib::CalculateChance(90) };
		if (value >= 0 && value <= 30) {
			m_PathStats[type].UnderlyingName = "extraBomb";
			m_PathStats[type].PowerupType = MathLib::EPowerupType::ExtraBomb;
		}
		if (value > 30 && value <= 60) {
			m_PathStats[type].UnderlyingName = "flames";
			m_PathStats[type].PowerupType = MathLib::EPowerupType::Flames;
		}
		if (value > 60 && value <= 90) {
			m_PathStats[type].UnderlyingName = "detonator";
			m_PathStats[type].PowerupType = MathLib::EPowerupType::Detonator;
		}
	}

	SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(Cellsize), static_cast<int>(Cellsize) };
	PathWay pathWay{
		id,
		component,
		m_PathStats[type],
		{ rect.x + rect.w / 2, rect.y + rect.h / 2 },
		rect,
	};

	//glm::vec2 pos{ pathWay.Rect.x - pathWay.Rect.w, pathWay.Rect.y };
	//pathWay.TextureComponent->SetPosition(pos);


	if (type == "spawn") {
		m_Spawns.push_back(pathWay);
	}
	else if (type == "enemySpawn") {
		m_EnemySpawns.push_back(pathWay);
	}

	m_Pathways.insert({ id, pathWay });
}

void dae::PathwayCreatorComponent::ActivatePathway(int id)
{
	if (m_Pathways.find(id) != m_Pathways.end()) {
		auto& path{ m_Pathways[id] };
		if (path.PathStats.PathType != MathLib::EPathType::Blocker) {

			path.PathStats.PathType = MathLib::EPathType::Tile;
			if (path.PathStats.HasUnderlyingThing) {
				path.TextureComponent->SetTexture("Levels/" + path.PathStats.UnderlyingName + ".png", 0.1f, 1, false, false);
			}
			else {
				path.PathStats.PathState = MathLib::EPathState::Tile;
				path.TextureComponent->SetIsVisible(false);
			}
		}
	}
}

void dae::PathwayCreatorComponent::ActivateBomb(int id)
{
	if (m_Pathways.find(id) != m_Pathways.end()) {
		auto& path{ m_Pathways[id] };
		path.TextureComponent->SetIsVisible(true);
		path.PathStats.PathType = MathLib::EPathType::Tile;
		path.PathStats.PathState = MathLib::EPathState::Explosion;
	}
}

void dae::PathwayCreatorComponent::PickDoorFromBreakableTiles()
{
	std::vector<int> breakablePathwayIds{};
	for (auto& pathWay : m_Pathways) {
		if (pathWay.second.PathStats.PathState == MathLib::EPathState::Breakable) {
			breakablePathwayIds.push_back(pathWay.first);
		}
	}

	int randomId{ MathLib::CalculateChance(static_cast<int>(breakablePathwayIds.size()) - 1)};
	auto& path{ m_Pathways.at(breakablePathwayIds[randomId]) };
	path.PathStats.PathState = MathLib::EPathState::Door;
	path.PathStats.UnderlyingName = "door";
	path.PathStats.HasUnderlyingThing = true;
	path.PathStats.PowerupType = MathLib::EPowerupType::Door;
}

void dae::PathwayCreatorComponent::Update()
{
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::HandleEntityTileOverlap()
{
	auto& children{ GetGameObject()->GetChildren() };
	if (m_pCharacters.size() > 0) {

		for (const auto& character : m_pCharacters) {
			if (character->IsMarkedForDestroy()) continue;
			const auto& moveComp{ character->GetComponent<EntityMovementComponent>() };
			for (const auto& gameObj : children)
			{
				HandleTileChange(character, gameObj, moveComp);
			}
		}
	}

	if (m_pEnemies.size() > 0) {
		for (const auto& enemy : m_pEnemies) {
			if (enemy->IsMarkedForDestroy()) continue;
			const auto& moveComp{ enemy->GetComponent<EntityMovementComponent>() };
			for (const auto& gameObj : children)
			{
				HandleTileChange(enemy, gameObj, moveComp);
			}
		}
	}
}

void dae::PathwayCreatorComponent::HandleTileChange(std::shared_ptr<GameObject> const& entity, dae::GameObject* const& path, dae::EntityMovementComponent* const& moveComp)
{
	auto texComp{ path->GetComponent<TextureComponent>() };
	if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCollider())) {
		moveComp->SetNextTileId(std::stoi(path->GetName()));
	}
	if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCharacterCollider())) {
		moveComp->SetCurrentTileId(std::stoi(path->GetName()));
	}

	auto& pathway{ m_Pathways.at(moveComp->GetCurrentTileId()) };
	if (pathway.PathStats.PathState == MathLib::EPathState::Explosion) {
		if (auto enemyComp{ entity->GetComponent<EnemyComponent>() }) {
			if (enemyComp->GetState() == MathLib::ELifeState::ALIVE) {
				enemyComp->SetState(new BombedState(m_pScene), MathLib::ELifeState::BOMBED);
			}
		}
		if (auto playerComp{ entity->GetComponent<PlayerComponent>() }) {
			if (playerComp->GetState() == MathLib::ELifeState::ALIVE) {
				playerComp->SetState(new BombedState(m_pScene), MathLib::ELifeState::BOMBED);
			}
		}
	}
	else if (pathway.PathStats.HasUnderlyingThing) {
		if (auto playerComp{ entity->GetComponent<PlayerComponent>() }) {
			if (playerComp->GetState() == MathLib::ELifeState::ALIVE) {
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
		{"tile", MathLib::EPathStats{ MathLib::EPathState::Tile, MathLib::EPathType::Tile, false, "blocker", "none" }},
		{"spawn", MathLib::EPathStats{ MathLib::EPathState::Spawn, MathLib::EPathType::Tile, false, "blocker", "none" }},
		{"enemySpawn", MathLib::EPathStats{ MathLib::EPathState::EnemySpawn, MathLib::EPathType::Tile, false, "blocker", "none" }},
		{"blocker", MathLib::EPathStats{ MathLib::EPathState::Blocker, MathLib::EPathType::Blocker, false, "blocker", "blocker" }},
		{"breakable", MathLib::EPathStats{ MathLib::EPathState::Breakable, MathLib::EPathType::Breakable, false, "breakable", "breakable" }},
		{"powerup", MathLib::EPathStats{ MathLib::EPathState::Powerup, MathLib::EPathType::Breakable, true, "breakable", "breakable" }},
	};
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::Render() const
{
	for (auto path : m_Pathways) {
		auto rrect = path.second.Rect;
		if (path.second.PathStats.PathType == MathLib::EPathType::Blocker) {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
		}
		else if (path.second.PathStats.PathType == MathLib::EPathType::Tile) {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 200, 0, 200);
		}
		else {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 200);
		}
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 100, 100, 0, 200);
		SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);
	}
}
