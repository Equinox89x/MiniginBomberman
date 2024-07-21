#pragma once 
#include "Component.h"
#include <map>
#include "TextureComponent.h"
#include "Scene.h"


namespace dae {

	struct PathWay {
		int id{ 0 };
		dae::TextureComponent* TextureComponent{ nullptr };
		MathLib::EPathStats PathStats{};
		const glm::vec2 Middle{};
		const SDL_Rect Rect{};

		void Clean() {
			delete TextureComponent;
			TextureComponent = nullptr;
		}
	};

	class EntityMovementComponent;
	class PathwayCreatorComponent : public Component
	{
	public:

		PathwayCreatorComponent() {};
		PathwayCreatorComponent(Scene* scene) : m_pScene{ scene } {
			m_pCharacters.clear();
			m_pEnemies.clear();
			m_Pathways.clear();
			m_Spawns.clear();
			m_EnemySpawns.clear();
			m_PathStats.clear();
		};
		virtual ~PathwayCreatorComponent() override;
		PathwayCreatorComponent(const PathwayCreatorComponent& other) = delete;
		PathwayCreatorComponent(PathwayCreatorComponent&& other) = delete;
		PathwayCreatorComponent& operator=(const PathwayCreatorComponent& other) = delete;
		PathwayCreatorComponent& operator=(PathwayCreatorComponent&& other) = delete;

		void AddPathway(int id, glm::vec2 pos, std::string type);
		void ActivatePathway(int id);
		void ActivateBomb(int id);
		void PickDoorFromBreakableTiles();

		virtual void Update() override;
		virtual void Init() override;
		virtual void Render() const override;

		std::vector<PathWay>& GetSpawns() { return m_Spawns; };
		std::vector<PathWay>& GetEnemySpawns() { return m_EnemySpawns; };

		const std::map<int, PathWay>& GetPathways() { return m_Pathways; };

	private:
		Scene* m_pScene{ nullptr };
		std::vector<GameObject*> m_pCharacters{ nullptr };
		std::vector<GameObject*> m_pEnemies{ nullptr };
		std::map<int, PathWay> m_Pathways{};
		std::vector<PathWay> m_Spawns{};
		std::vector<PathWay> m_EnemySpawns{};
		std::map<std::string, MathLib::EPathStats> m_PathStats{};
		int* m_Ptr = reinterpret_cast<int*>(0xdddddddddddddddd);

		SDL_Rect m_LeftMapBorder{}, m_RightMapBorder{}, m_TopMapBorder{}, m_BottomMapBorder{};

		void HandleEntityTileOverlap();

		void HandleTileChange(GameObject* entity, dae::GameObject* path, dae::EntityMovementComponent* moveComp);

	};
}
