#include "TransformComponent.h"
#include <glm/gtx/rotate_vector.hpp> 

dae::TransformComponent::TransformComponent() :
	m_Position{ 0, 0, 0 },
	m_WorldPosition{ 0, 0, 0 },
	m_Rotation{ 0, 0, 0 },
	m_WorldRotation{ 0, 0, 0 }
{}

void dae::TransformComponent::UpdateTransforms()
{
	if (GetGameObject()) {
		if (const GameObject * parent{ GetGameObject()->GetParent() }) {
			auto pos{ parent->GetTransform()->GetPosition() };
			auto rot{ parent->GetTransform()->GetRotation() };
			m_WorldRotation = rot + GetRotation();
			m_WorldPosition = pos + GetPosition();
/*			m_Rotation += m_WorldRotation;
			m_Position += m_WorldPosition;		*/	
			m_WorldRotation += m_Rotation;
			m_WorldPosition += m_Position;
		}
		else {
			//m_Rotation += m_WorldRotation;
			//m_Position += m_WorldPosition;
		}
		for (auto child : GetGameObject()->GetChildren()) {
			child->GetTransform()->UpdateTransforms();
		}
	}


	//// Define the rotation angle in radians
	//float angle = glm::radians(45.0f);

	//// Create a rotation matrix using glm::rotate
	//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	//// Apply the rotation to the initial vector
	//m_Position = rotationMatrix * glm::vec4(1,1,1, 1.0f);



	m_IsDirty = false;
}

void dae::TransformComponent::SetDirty()
{
	m_IsDirty = true;
	for (const auto gameObject : GetGameObject()->GetChildren()) {
		if (!gameObject->GetTransform()->IsDirty()) {
			gameObject->GetTransform()->SetDirty();
		}
	}
}

bool dae::TransformComponent::CheckIfDirty()
{
	//If Parent is dirty == update required (spatial relation)
	if (const GameObject* pParent = GetGameObject()->GetParent())
	{
		if (pParent->GetTransform()->IsDirty())
		{
			return true;
		}
	}

	return false;
}

void dae::TransformComponent::Update()
{
	m_IsDirty = CheckIfDirty();

	if (m_IsDirty)
		UpdateTransforms();

}

void dae::TransformComponent::Translate(glm::vec3 pos) { Translate(pos.x, pos.y, pos.z); }
void dae::TransformComponent::Translate(glm::vec2 pos) { Translate(pos.x, pos.y, 0); }
void dae::TransformComponent::Translate(float x, float y) { Translate(x, y, 0); };
void dae::TransformComponent::Translate(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
	UpdateTransforms();
}

void dae::TransformComponent::SetScale(glm::vec3 pos) { SetScale(pos.x, pos.y, pos.z); }
void dae::TransformComponent::SetScale(glm::vec2 pos) { SetScale(pos.x, pos.y, 1); }
void dae::TransformComponent::SetScale(float x, float y) { SetScale(x, y, 1); };
void dae::TransformComponent::SetScale(float scale) { SetScale(scale, scale, scale); };
void dae::TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void dae::TransformComponent::TranslateWorld(glm::vec3 pos) { TranslateWorld(pos.x, pos.y, pos.z); }
void dae::TransformComponent::TranslateWorld(glm::vec2 pos) { TranslateWorld(pos.x, pos.y, 0); }
void dae::TransformComponent::TranslateWorld(float x, float y) { TranslateWorld(x, y, 0); };
void dae::TransformComponent::TranslateWorld(float x, float y, float z)
{
	m_WorldPosition.x = x;
	m_WorldPosition.y = y;
	m_WorldPosition.z = z;
	UpdateTransforms();
}

void dae::TransformComponent::AddTranslate(glm::vec3 pos) { AddTranslate(pos.x, pos.y, pos.z); }
void dae::TransformComponent::AddTranslate(glm::vec2 pos) { AddTranslate(pos.x, pos.y, 0); }
void dae::TransformComponent::AddTranslate(float x, float y) { AddTranslate(x, y, 0); };
void dae::TransformComponent::AddTranslate(float x, float y, float z)
{
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;
	m_IsDirty = true;
	UpdateTransforms();
}

void dae::TransformComponent::AddTranslateWorld(glm::vec3 pos) { AddTranslateWorld(pos.x, pos.y, pos.z); }
void dae::TransformComponent::AddTranslateWorld(glm::vec2 pos) { AddTranslateWorld(pos.x, pos.y, 0); }
void dae::TransformComponent::AddTranslateWorld(float x, float y) { AddTranslateWorld(x, y, 0); }
void dae::TransformComponent::AddTranslateWorld(float x, float y, float z)
{
	m_WorldPosition.x += x;
	m_WorldPosition.y += y;
	m_WorldPosition.z += z;
	UpdateTransforms();
}

void dae::TransformComponent::Rotate(const float angle) { Rotate(0, angle, 0); };
void dae::TransformComponent::Rotate(float x, float y, float z)
{
	//auto rotationCenter{ GetGameObject()->GetTransform()->GetWorldPosition() };
	//auto rotationCenterOffset{ GetGameObject()->GetTransform()->GetPosition() };

	//m_Rotation.x = x;
	//m_Rotation.y = y;
	//m_Rotation.z = z;

	//// Define the rotation angle in radians
	//float angle = glm::radians(y);

	//// Create a rotation matrix using glm::rotate
	//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1.f, 0));

	//// Apply the rotation to the initial vector
	//glm::vec3 rotatedVector = rotationMatrix * glm::vec4(1, 1, 1, 1);
	//m_Position += rotatedVector;
	//UpdateTransforms();


	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;

	//float angle = glm::radians(y); 

	//// Create a rotation matrix using GLM
	//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1, 0));

	//// Translate the object so that the center becomes the origin
	//glm::vec3 translatedObject = m_Position - m_WorldPosition;

	//// Apply rotation to the translated object
	//glm::vec4 rotatedObject = rotationMatrix * glm::vec4(translatedObject, 1.0f);

	//// Translate the rotated object back
	//glm::vec3 finalObject = glm::vec3(rotatedObject) + m_WorldPosition;


	//m_RotPosition = 
	if (GetGameObject()->GetParent()) {
		m_Position = GetGameObject()->GetParent()->GetTransform()->GetFullPosition() + Rotate(0, 0, y, m_Offset, true);
		
	}
	else {	
		m_Position = Rotate(0, 0, y, m_Offset, true);
	}
	// Update transforms
	//UpdateTransforms();
}

const glm::vec3 dae::TransformComponent::GetFullPosition() const
{
	return m_WorldPosition + m_Position;
}

const glm::vec3 dae::TransformComponent::GetWorldPosition()
{
	glm::vec3 pos{ m_Position };
	if (m_pGameObject) {
		if (m_pGameObject->GetParent())
			pos += m_pGameObject->GetParent()->GetTransform()->GetWorldPosition();
	}
	return pos;
}

const glm::vec3 dae::TransformComponent::GetWorldRotation() const
{
	glm::vec3 rot{ m_Rotation };
	if (m_pGameObject) {
		if (m_pGameObject->GetParent())
			rot += m_pGameObject->GetParent()->GetTransform()->GetWorldRotation();
	}
	return rot;
}


glm::vec3 dae::TransformComponent::Rotate(const float cx, const float cy, float angle, glm::vec3 offset, bool isDegrees)
{
	if (isDegrees) {
		angle = glm::radians(angle);
	}
	float s = sinf(angle);
	float c = cosf(angle);

	// translate point back to origin:
	offset.x -= cx;
	offset.y -= cy;

	// rotate point
	float xnew = offset.x * c - offset.y * s;
	float ynew = offset.x * s + offset.y * c;

	// translate point back:
	offset.x = xnew + cx;
	offset.y = ynew + cy;
	return offset;
}