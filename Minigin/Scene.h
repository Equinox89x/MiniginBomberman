#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	//class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject* object);
		void RemoveAll();

		GameObject* GetGameObject(std::string name);
		std::vector<GameObject*> GetGameObjects(std::string name, bool isCompleteWord = true);
		std::vector<GameObject*> GetGameObjects();

		void Init() const;
		void LateUpdate();
		void Update( );
		void Render() const;
		void Cleanup();

		void SetActive(bool isActive) { m_IsActive = isActive; };
		bool GetIsActive() { return m_IsActive; };

		std::string GetName() { return m_name; };

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);
		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_objects{};
		bool m_IsActive{ false };

		static unsigned int m_idCounter; 
	};

}
