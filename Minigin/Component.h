#pragma once
#include <string>

namespace dae
{
	class GameObject;
	class Subject;
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Init() {};
		virtual void LateUpdate() {};
		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void Render() const {};

		GameObject* GetGameObject() const { return m_pGameObject; };
		std::string GetName() { return Name; };
		void SetName(std::string name) { Name = name; };

		void MarkForDestroy() { m_MarkedForDelete = true; };
		bool IsMarkedForDestroy() { return m_MarkedForDelete; };

	protected:
		GameObject* m_pGameObject{};
		Subject* m_pCallback{};

	private:
		friend class GameObject;
		std::string Name{ "" };
		bool m_MarkedForDelete{ false };
	};
}

