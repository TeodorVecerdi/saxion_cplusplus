#pragma once
#include <functional>

#include "Core/Scene/ScriptableBehaviour.hpp"

class TextRenderer;
class SpriteRenderer;

class Button final : public ScriptableBehaviour {
public:
    Button(SpriteRenderer& sprite);
    Button(SpriteRenderer& sprite, std::function<void()> onClick);
    Button(const std::string& identifier, SpriteRenderer& sprite, std::function<void()> onClick);
    void setSprite(SpriteRenderer* sprite);
    void setLabel(TextRenderer* label);
    void setOnClick(std::function<void()> onClick);
    void setEnabled(bool enabled);
protected:
    void onUpdate(sf::Time ts) override;
private:
    std::function<void()> onClick;
    SpriteRenderer* sprite;
    TextRenderer* label;
    bool enabled;
};
