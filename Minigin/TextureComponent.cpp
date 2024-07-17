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
    auto dt{ Timer::GetInstance().GetDeltaTime() };

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
    const auto& pos{ GetGameObject()->GetTransform()->GetFullPosition() };
    const auto& scale{ GetGameObject()->GetTransform()->GetScale() };

    SDL_QueryTexture(m_Texture.m_pTexture->GetSDLTexture(), nullptr, nullptr, &m_SrcRect.w, &m_SrcRect.h);
    if (m_Texture.NrOfFrames > 1) {
        m_SrcRect.w /= m_Texture.NrOfFrames;
        m_SrcRect.x = m_SrcRect.w * (m_Texture.CurrentFrame - 1);
    }
    m_DstRect = { 0, 0, m_SrcRect.w, m_SrcRect.h };

    if (m_Texture.NrOfFrames > 1) m_DstRect.x = m_SrcRect.w * m_Texture.CurrentFrame;
    m_DstRect.x += static_cast<int>(pos.x + Offset.x) - m_SrcRect.x;
    m_DstRect.y = static_cast<int>(pos.y + Offset.y);

    m_DstRect.w = static_cast<int>(m_SrcRect.w * scale.x);
    m_DstRect.h = static_cast<int>(m_SrcRect.h * scale.y);

    m_Rect = m_DstRect;
}

void dae::TextureComponent::Render() const
{
    if (m_IsVisible) {
        if (m_Texture.NrOfFrames > 1) {
            SDL_Point center = { m_SrcRect.w / 2, m_SrcRect.h / 2 };
            SDL_RenderCopyEx(Renderer::GetInstance().GetSDLRenderer(), m_Texture.m_pTexture->GetSDLTexture(), &m_SrcRect, &m_DstRect, Angle, nullptr, SDL_FLIP_NONE);
        }
        else {
            SDL_RenderCopyEx(Renderer::GetInstance().GetSDLRenderer(), m_Texture.m_pTexture->GetSDLTexture(), nullptr, &m_DstRect, Angle, nullptr, SDL_FLIP_NONE);
        }
    }

    //SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
    //SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &m_Rect); // D
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

void dae::TextureComponent::SetTexture(TextureData& textureData)
{
    m_Texture = textureData;
}

void dae::TextureComponent::SetTexture(MathLib::Movement movement, const std::string& filename, float animSpeed, int nrOfFrames, bool resetAnim, bool canProgress)
{
    SetTexture(filename, animSpeed, nrOfFrames, resetAnim, canProgress);
    FileNames.insert({ movement, m_Texture });
}

void dae::TextureComponent::RemoveTexture(MathLib::Movement movement)
{
    FileNames.erase(movement);
    if (FileNames.size() > 0) {
        SetTexture(FileNames.begin()->second);
    }
}

void dae::TextureComponent::SetPosition(const float x, const float y)
{
    GetGameObject()->GetTransform()->Translate(x, y, 0.0f);
    m_needsUpdate = true;
}

void dae::TextureComponent::AddPosition(const float x, const float y)
{
    GetGameObject()->GetTransform()->AddTranslate(x, y, 0.0f);
    m_needsUpdate = true;
}

void dae::TextureComponent::SetWorldPosition(const float x, const float y)
{
    GetGameObject()->GetTransform()->TranslateWorld(x, y, 0.0f);
    m_needsUpdate = true;
}

void dae::TextureComponent::Scale(const float x, const float y)
{
    GetGameObject()->GetTransform()->SetScale(x, y, 1);
    m_needsUpdate = true;
}


void dae::TextureComponent::Rotate(const float angle)
{
    GetGameObject()->GetTransform()->Rotate(angle);
    Angle = angle;
    m_needsUpdate = true;
}

