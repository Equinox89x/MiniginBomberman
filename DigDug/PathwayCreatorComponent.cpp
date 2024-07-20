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
	comp->SetTexture("Levels/blocker.png");
	auto component{ go->AddComponent(std::move(comp)) };
	//component->SetPosition(pos.x, pos.y);
	component->SetIsVisible(false);

	SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(Cellsize), static_cast<int>(Cellsize )};
	PathWay pathWay{
		id,
		component,
		MathLib::EPathState::Blocker,
		{ rect.x + rect.w / 2, rect.y + rect.h / 2 },
		rect,
	};

	//glm::vec2 pos{ pathWay.Rect.x - pathWay.Rect.w, pathWay.Rect.y };
	//pathWay.TextureComponent->SetPosition(pos);


	if(type == "tile"){
		pathWay.PathState = MathLib::EPathState::Tile;
	}
	else if (type == "spawn") {
		pathWay.PathState = MathLib::EPathState::Spawn;
		m_Spawns.push_back(pathWay);
	}
	else if(type == "enemySpawn"){
		pathWay.PathState = MathLib::EPathState::EnemySpawn;
		m_EnemySpawns.push_back(pathWay);
	}
	else if (type == "blocker") {
		pathWay.PathState = MathLib::EPathState::Blocker;
		component->SetIsVisible(true);
	}	
	else if (type == "breakable") {
		pathWay.PathState = MathLib::EPathState::Breakable;
		component->SetTexture("Levels/breakable.png");
		component->SetIsVisible(true);
	}

	m_Pathways.insert({ id, pathWay });
}

void dae::PathwayCreatorComponent::ActivatePathway(int id)
{
	if (m_Pathways.find(id) != m_Pathways.end()) {
		auto& path{ m_Pathways[id] };
		if (path.PathState != MathLib::EPathState::Blocker) {
			path.TextureComponent->SetIsVisible(false);
			path.PathState = MathLib::EPathState::Tile;
		}
	}
}

void dae::PathwayCreatorComponent::ActivateBomb(int id)
{
	if (m_Pathways.find(id) != m_Pathways.end()) {
		m_Pathways[id].TextureComponent->SetIsVisible(true);
		m_Pathways[id].PathState = MathLib::EPathState::Explosion;
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

	if (m_Pathways.at(moveComp->GetCurrentTileId()).PathState == MathLib::EPathState::Explosion) {
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
}

void dae::PathwayCreatorComponent::Init()
{
	m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
	m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::Render() const
{
	for (auto path : m_Pathways) {
		auto rrect = path.second.Rect;
		if (path.second.PathState == MathLib::EPathState::Blocker) {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
		}
		else if(path.second.PathState == MathLib::EPathState::Tile){
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
