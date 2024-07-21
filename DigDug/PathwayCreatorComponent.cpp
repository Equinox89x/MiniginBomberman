#include "PathwayCreatorComponent.h"
#include "GameObject.h"
#include "EntityMovementComponent.h"
#include "Renderer.h"
#include <memory>
#include <iostream>
#include "EnemyComponent.h"
#include "PlayerComponent.h"
#include <ValuesComponent.h>

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
			if (path.PathStats.HasPowerup) {
				path.TextureComponent->SetTexture("Levels/" + path.PathStats.PowerupName + ".png", 0.1f, 1, false, false);
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

	if (m_Pathways.at(moveComp->GetCurrentTileId()).PathStats.PathState == MathLib::EPathState::Explosion) {
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
	else if (m_Pathways.at(moveComp->GetCurrentTileId()).PathStats.PathState == MathLib::EPathState::Powerup) {
		if (auto playerComp{ entity->GetComponent<PlayerComponent>() }) {
			if (playerComp->GetState() == MathLib::ELifeState::ALIVE) {
				playerComp->ActivatePowerup(m_Pathways.at(moveComp->GetCurrentTileId()));
			}
		}
	}
}

void dae::PathwayCreatorComponent::Init()
{
	m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
	m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);
	m_PathStats = std::map<std::string, MathLib::EPathStats>{
		{"tile", MathLib::EPathStats{ MathLib::EPathState::Tile, MathLib::EPathType::Tile, MathLib::EPowerupType::None, false, "blocker", "none" }},
		{"spawn", MathLib::EPathStats{ MathLib::EPathState::Spawn, MathLib::EPathType::Tile, MathLib::EPowerupType::None, false, "blocker", "none" }},
		{"enemySpawn", MathLib::EPathStats{ MathLib::EPathState::EnemySpawn, MathLib::EPathType::Tile, MathLib::EPowerupType::None, false, "blocker", "none" }},
		{"blocker", MathLib::EPathStats{ MathLib::EPathState::Blocker, MathLib::EPathType::Blocker, MathLib::EPowerupType::None, false, "blocker", "blocker" }},
		{"breakable", MathLib::EPathStats{ MathLib::EPathState::Breakable, MathLib::EPathType::Breakable, MathLib::EPowerupType::None, false, "breakable", "breakable" }},
		{"extraBomb", MathLib::EPathStats{ MathLib::EPathState::Powerup, MathLib::EPathType::Breakable, MathLib::EPowerupType::ExtraBomb, true, "breakable", "extraBomb" }},
		{"detonator", MathLib::EPathStats{ MathLib::EPathState::Powerup, MathLib::EPathType::Breakable, MathLib::EPowerupType::Detonator, true, "breakable", "detonator" }},
		{"flames", MathLib::EPathStats{ MathLib::EPathState::Powerup, MathLib::EPathType::Breakable, MathLib::EPowerupType::Flames, true, "breakable", "flames" }}
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
