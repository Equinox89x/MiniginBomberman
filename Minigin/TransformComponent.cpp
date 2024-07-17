#include "TransformComponent.h"

dae::TransformComponent::TransformComponent()
	: m_TransformChanges{ TransformChange::None }
	, m_Local{}
	, m_World{}
{
}

void dae::TransformComponent::Init()
{
	m_TransformChanges = 7; //all bitmasks
	Update();
}

void dae::TransformComponent::Render() const
{
}

void dae::TransformComponent::Update()
{
	if (m_TransformChanges == TransformChange::None)
		return;

	if (m_TransformChanges & TransformChange::Translation)
	{
		CalculateWorldPosition();
	}
	if (m_TransformChanges & TransformChange::Rotation)
	{
		CalculateWorldRotation();
		CalculateFwdRight();
	}
	if (m_TransformChanges & TransformChange::Scale)
	{
		CalculateWorldScale();
	}

	m_TransformChanges = TransformChange::None;
}

void dae::TransformComponent::Translate(float x, float y)
{
	m_Local.Position.x += x;
	m_Local.Position.y += y;
	m_TransformChanges |= TransformChange::Translation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Translation;
}

void dae::TransformComponent::Translate(const glm::vec2& vec2)
{
	m_Local.Position += glm::vec3{vec2.x,vec2.y,0.f};
	m_TransformChanges |= TransformChange::Translation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Translation;
}

void dae::TransformComponent::Translate(const glm::vec3& vec3)
{
	m_Local.Position += vec3;
	m_TransformChanges |= TransformChange::Translation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Translation;
}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_Local.Position.x = x;
	m_Local.Position.y = y;
	m_Local.Position.z = z;
	m_TransformChanges |= TransformChange::Translation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Translation;
}

void dae::TransformComponent::SetPosition(const glm::vec2& vec2, float z)
{
	m_Local.Position.x = vec2.x;
	m_Local.Position.y = vec2.y;
	m_Local.Position.z = z;
	m_TransformChanges |= TransformChange::Translation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Translation;
}

void dae::TransformComponent::SetPosition(const glm::vec3& vec3)
{
	m_Local.Position = vec3;
	m_TransformChanges |= TransformChange::Translation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Translation;
}

void dae::TransformComponent::Rotate(float r)
{
	m_Local.Rotation += r;
	m_TransformChanges |= TransformChange::Rotation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Rotation;
}

void dae::TransformComponent::SetRotation(float r)
{
	m_Local.Rotation = r;
	m_TransformChanges |= TransformChange::Rotation;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Rotation;
}

void dae::TransformComponent::SetScale(float x, float y)
{
	m_Local.Scale.x = x;
	m_Local.Scale.y = y;
	m_TransformChanges |= TransformChange::Scale;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Scale;
}

void dae::TransformComponent::SetScale(const glm::vec2& scale)
{
	m_Local.Scale = scale;
	m_TransformChanges |= TransformChange::Scale;

	for (GameObject* pChild : GetGameObject()->GetChildren())
		pChild->GetTransform()->m_TransformChanges |= TransformChange::Scale;
}

void dae::TransformComponent::CalculateWorldPosition()
{
	GameObject* pParent{ GetGameObject()->GetParent() };

	if (!pParent)
	{
		m_World.Position = m_Local.Position;
		return;
	}

	m_World.Position.x = 0.f;
	m_World.Position.y = 0.f;

	//calculate pivot rotational point
	const float parentRot = -pParent->GetTransform()->GetLocal().Rotation * (3.14159265359f / 180.f);
	const float sinCalc{ sinf(parentRot) };
	const float cosCalc{ cosf(parentRot) };
	const float newX{ m_World.Position.x * cosCalc - m_World.Position.y * sinCalc };
	m_World.Position.y = m_World.Position.y * cosCalc + m_World.Position.x * sinCalc;
	m_World.Position.x = newX;

	m_World.Position += pParent->GetTransform()->GetWorld().Position;

	m_World.Position += m_Local.Position;
}

void dae::TransformComponent::CalculateWorldRotation()
{
	GameObject* pParent{ GetGameObject()->GetParent() };

	if (!pParent)
	{
		m_World.Rotation = m_Local.Rotation;
		return;
	}

	float& finalRot{ m_World.Rotation };
	finalRot = 0.f;

	finalRot += pParent->GetTransform()->GetLocal().Rotation;

	finalRot += pParent->GetTransform()->GetWorld().Rotation;

	finalRot += m_Local.Rotation;
}

void dae::TransformComponent::CalculateWorldScale()
{
	GameObject* pParent{ GetGameObject()->GetParent() };

	if (!pParent)
	{
		m_World.Scale = m_Local.Scale;
		return;
	}

	glm::vec2 finalScale{ m_World.Scale };
	finalScale = m_Local.Scale;

	const glm::vec2 parentWorldScale = pParent->GetTransform()->GetWorld().Scale;
	finalScale.x *= parentWorldScale.x;
	finalScale.y *= parentWorldScale.y;
}

void dae::TransformComponent::CalculateFwdRight()
{
	//WorldRight
	//m_Forward.x = 1.f * cos - 0.f * sin;
	//m_Forward.y = 1.f * sin + 0.f * cos;

	//const float cos = cosf(m_Transform.Rotation);
	//const float sin = sinf(m_Transform.Rotation);

	m_Local.Forward.x = cosf(m_Local.Rotation);
	m_Local.Forward.y = sinf(m_Local.Rotation);
	//m_Right = Math2D::OrthogonalR(m_Forward);
	m_Local.Right.x = m_Local.Forward.y;
	m_Local.Right.y = -m_Local.Forward.x;

	m_World.Forward.x = cosf(m_World.Rotation);
	m_World.Forward.y = sinf(m_World.Rotation);

	m_World.Right.x = m_World.Forward.y;
	m_World.Right.y = -m_World.Forward.x;
}