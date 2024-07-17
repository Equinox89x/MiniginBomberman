#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		std::shared_ptr<GameObject> GetGameObject(std::string name);
		std::vector<std::shared_ptr<GameObject>> GetGameObjects(std::string name, bool isCompleteWord = true);
		std::vector<std::shared_ptr<GameObject>>& GetGameObjects();

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
		std::vector < std::shared_ptr<GameObject>> m_objects{};
		bool m_IsActive{ false };

		static unsigned int m_idCounter; 
	};

}
