#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Component.h"
#include "TransformComponent.h"


//int GridSize = 30;
//int GridSizeX = 30;

#define WindowSizeX 640
#define WindowSizeY 640
//#define GridSize 30
//#define GridSizeX 30
#define GridSizeY 13
//#define WindowBuffer 80

#define Margin 40.f
#define SubMargin Margin * 2

#define Cellsize WindowSizeX/16
#define PlayerSize 14.f

const enum Names {
	Player0,
	Player1,
	PlayerGeneral,
	EnemyGeneral,
	EnemyHolder,
	PumpGeneral,
	Global,
	PathCreator,
	ScoreHolder,
	Life,
	ScoreGeneral,
	Score1,
	Score2,
	Selector,
	MainMenu,
	Background,

	SoloLevelNameGeneral,
	SoloLevelName0,
	SoloLevelName1,
	SoloLevelName2,
	CoopLevelName,
	VersusLevelName,
	HighScoreName,
	Bomb,
	NUM_ENUM_VALUES
};

const std::string EnumStrings[NUM_ENUM_VALUES] = {
	"Player0", "Player1", "Player", "Enemy", "EnemyHolder", "Pump", "Global", "PathCreator", "ScoreHolder", "Life", "Score", "Score0", "Score1", "Selector", "MainMenu", "Background",
	"BombermanSolo", "BombermanSolo0", "BombermanSolo1", "BombermanSolo2", "BombermanCoop", "BombermanVersus","HighScoreScreen", "Bomb",
};

namespace dae
{
	//class Component;
	class TransformComponent;

	class GameObject final
	{
	public:

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Init();
		virtual void LateUpdate();
		virtual void Update();
		virtual void Render() const;
		void Cleanup();

		void MarkForDestroy() {
			Cleanup();
			m_MarkedForDelete = true;
		};
		bool IsMarkedForDestroy() { return m_MarkedForDelete; };

		template<typename Comp>
		Comp* AddComponent(std::unique_ptr<Comp> comp) {
			if (comp) {
				comp->m_pGameObject = this;
				dynamic_cast<Component*>(comp.get())->Init();
				m_pComponents.push_back(std::move(comp));
				return GetComponent<Comp>();
			}
			return nullptr;
		}

		template<typename Comp>
		Comp* GetComponent() {
			const type_info& ti = typeid(Comp);
			for (const auto& component : m_pComponents) {
				if (component && typeid(*component.get()) == ti)
					return static_cast<Comp*>(component.get());
			}
			return nullptr;
		}

		template<typename Comp>
		Comp* GetComponent(std::string name) {
			const type_info& ti = typeid(Comp);
			for (const auto& component : m_pComponents)
			{
				if (component && typeid(*component.get()) == ti && component->GetName() == name)
					return static_cast<Comp*>(component.get());
			}
			return nullptr;
		}

		template<typename Comp>
		void RemoveComponent(Comp* componentPtr) {
			const std::type_info& ti = typeid(Comp);
			for (auto it = m_pComponents.begin(); it != m_pComponents.end(); ++it) {
				if (*it && typeid(**it) == ti && it->get() == componentPtr) {
					it = m_pComponents.erase(it); // Remove the component from the vector
					return;
				}
			}
		}
		void RemoveComponent(const std::unique_ptr<Component>& comp);

		void SetParent(GameObject* const parent, bool updateTransforms = true);
		GameObject* GetParent() const;
		void RemoveChild(GameObject* pObject);
		dae::GameObject* AddChild(GameObject* go, bool updateTransforms = true);
		void SetName(std::string name) { m_ObjectName = name; };
		std::string GetName() { return m_ObjectName; };

		const glm::vec2 GetCenter() const { return m_Center; };
		TransformComponent* GetTransform() const { return m_pTransform; };
		std::vector<GameObject*>& GetChildren() { return m_pChildren; };
		GameObject* GetChild(std::string name);
		std::vector<GameObject*> GetChildren(std::string name);

	private:
		std::vector<GameObject*> m_pChildren{ std::vector<GameObject*>() };
		std::vector<std::unique_ptr<Component>> m_pComponents{ };
		GameObject* m_pParent{ nullptr };
		TransformComponent* m_pTransform{};
		std::string m_ObjectName{};
		glm::vec2 m_Center{};

		bool m_MarkedForDelete{ false };
	};
}
