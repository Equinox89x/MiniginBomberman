#include "TextureComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Timer.h"


dae::TextureComponent::~TextureComponent()
{
    m_Texture.m_pTexture.reset();
}

void dae::TextureComponent::Update()
{
    const auto dt{ Timer::GetInstance().GetDeltaTime() };

    if (m_Texture.CanProgress) {
        m_Texture.AnimTimer -= dt;
        if (m_Texture.AnimTimer <= 0) {
            m_Texture.CurrentFrame++;
            m_Texture.AnimTimer = m_Texture.DefaultAnimTime;
        }
        if (m_Texture.CurrentFrame > m_Texture.NrOfFrames) {
            m_Texture.CurrentFrame = 1;
        }
    }

    HandleAnimation();

    if (m_CanRotate) {
        Angle += dt * m_RotationSpeed;
    }
}
void dae::TextureComponent::HandleAnimation() {
    if (!GetGameObject()) return;
    const auto& pos{ GetGameObject()->GetTransform()->GetWorld().Position };
    const auto& scale{ GetGameObject()->GetTransform()->GetWorld().Scale };

    SDL_QueryTexture(m_Texture.m_pTexture->GetSDLTexture(), nullptr, nullptr, &m_SrcRect.w, &m_SrcRect.h);
    m_SrcRect.w /= m_Texture.NrOfFrames;
    m_SrcRect.x = m_SrcRect.w * (m_Texture.CurrentFrame - 1);
    m_DstRect = { 0, 0, m_SrcRect.w, m_SrcRect.h };

    m_DstRect.x = m_SrcRect.w * (m_Texture.CurrentFrame-1);
    m_DstRect.x += static_cast<int>(pos.x) - m_SrcRect.x;
    m_DstRect.y = static_cast<int>(pos.y);

    m_DstRect.w = static_cast<int>(m_SrcRect.w * scale.x);
    m_DstRect.h = static_cast<int>(m_SrcRect.h * scale.y);

    m_Rect = m_DstRect;
}

void dae::TextureComponent::Render() const
{
    if (m_IsVisible) {
        SDL_RenderCopyEx(Renderer::GetInstance().GetSDLRenderer(), m_Texture.m_pTexture->GetSDLTexture(), &m_SrcRect, &m_DstRect, Angle, nullptr, SDL_FLIP_NONE);
    }
}

void dae::TextureComponent::SetTexture(const std::string& filename, float animSpeed, int nrOfFrames, bool resetAnim, bool canProgress)
{
    if (m_Texture.FileName != filename) {
        m_Texture.FileName = filename;
        m_Texture.m_pTexture.reset();
        m_Texture.m_pTexture = std::move(ResourceManager::GetInstance().LoadTexture(filename));
        m_Texture.NrOfFrames = nrOfFrames;
        m_Texture.DefaultAnimTime = animSpeed;
        m_Texture.CanProgress = canProgress;
        if (resetAnim) {
            m_Texture.CurrentFrame = 1;
            m_Texture.AnimTimer = m_Texture.DefaultAnimTime;
        }
    }
}

void dae::TextureComponent::SetTexture(const TextureData& textureData)
{
    m_Texture = textureData;
}

void dae::TextureComponent::SetTexture(MathLib::EMovement movement, const std::string& filename, float animSpeed, int nrOfFrames, bool resetAnim, bool canProgress)
{
    SetTexture(filename, animSpeed, nrOfFrames, resetAnim, canProgress);
    FileNames.insert({ movement, m_Texture });
}

void dae::TextureComponent::RemoveTexture(MathLib::EMovement movement)
{
    FileNames.erase(movement);
    if (FileNames.size() > 0) {
        SetTexture(FileNames.begin()->second);
    }
}

void dae::TextureComponent::Rotate(const float angle)
{
	GetGameObject()->GetTransform()->Rotate(angle);
	Angle = angle;
	m_needsUpdate = true;
}