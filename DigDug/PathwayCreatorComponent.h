#pragma once 
#include "Component.h"
#include <map>
#include "TextureComponent.h"
#include "Scene.h"

namespace dae {

	enum class EPathState {
		Tile, Blocker, Spawn, EnemySpawn, Breakable, Bomb
	};

	struct PathWay {
		int id{ 0 };
		dae::TextureComponent* TextureComponent{ nullptr };
		EPathState PathState{ EPathState::Blocker };
		const glm::vec2 Middle{};
		const SDL_Rect Rect{};

		void Clean() {
			delete TextureComponent;
			TextureComponent = nullptr;
		}
	};

	class PathwayCreatorComponent : public Component
	{
	public:
		PathwayCreatorComponent() {};
		PathwayCreatorComponent(Scene* scene) : m_pScene{ scene } {
		};
		virtual ~PathwayCreatorComponent() override;
		PathwayCreatorComponent(const PathwayCreatorComponent& other) = delete;
		PathwayCreatorComponent(PathwayCreatorComponent&& other) = delete;
		PathwayCreatorComponent& operator=(const PathwayCreatorComponent& other) = delete;
		PathwayCreatorComponent& operator=(PathwayCreatorComponent&& other) = delete;

		void AddPathway(int id, glm::vec2 pos, std::string type);
		void ActivatePathway(int id);
		void ActivateBomb(int id);
		virtual void Update() override;
		virtual void Init() override;
		virtual void Render() const override;

		std::vector<PathWay>& GetSpawns() { return m_Spawns; };
		std::vector<PathWay>& GetEnemySpawns() { return m_EnemySpawns; };

		const std::map<int, PathWay>& GetPathways() { return m_Pathways; };

		private:
			Scene* m_pScene{ nullptr };
			std::vector<std::shared_ptr<GameObject>> m_pCharacters{ nullptr };
			std::vector<std::shared_ptr<GameObject>> m_pEnemies{ nullptr };
			std::map<int, PathWay> m_Pathways{};
			std::vector<PathWay> m_Spawns{};
			std::vector<PathWay> m_EnemySpawns{};

			SDL_Rect m_LeftMapBorder{}, m_RightMapBorder{}, m_TopMapBorder{}, m_BottomMapBorder{};

			void HandleEntityTileOverlap();

	};
}
