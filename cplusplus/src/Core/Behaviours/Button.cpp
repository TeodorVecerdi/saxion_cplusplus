#include "Button.hpp"
#include "SpriteRenderer.hpp"
#include "Core/Input.hpp"
#include "Core/Drawing/Theme.hpp"

Button::Button(SpriteRenderer& sprite) : Button(default_identifier(this), sprite, nullptr)  {}

Button::Button(SpriteRenderer& sprite, std::function<void()> onClick) : Button(default_identifier(this), sprite, onClick) {}

Button::Button(const std::string& identifier, SpriteRenderer& sprite, std::function<void()> onClick) : ScriptableBehaviour(identifier) {
    setSprite(&sprite);
    label = nullptr;
    this->onClick = onClick;
    enabled = true;
}

void Button::setSprite(SpriteRenderer* sprite) {
    ASSERT(sprite, "SpriteRenderer of Button[" << getIdentifier() << "] cannot be null");
    this->sprite = sprite;
}

void Button::setLabel(TextRenderer* label) {
    this->label = label;
}

void Button::setOnClick(const std::function<void()> onClick) {
    this->onClick = onClick;
}

void Button::setEnabled(bool enabled) {
    this->enabled = enabled;
}

void Button::onUpdate(const sf::Time ts) {
    auto* activeTheme = Theme::activeTheme;

    if(!enabled) {
        sprite->setColor(activeTheme->buttonDisabled);
        return;
    }

    const auto spriteRect = sprite->getGlobalBounds();
    const bool isHovering = spriteRect.contains(static_cast<float>(Input::mouseX), static_cast<float>(Input::mouseY));
    if (isHovering && Input::GetMouseButton(0)) {
        sprite->setColor(activeTheme->buttonActive);
    } else if (isHovering) {
        sprite->setColor(activeTheme->buttonHover);
    } else {
        sprite->setColor(activeTheme->buttonNormal);
    }
    if (isHovering && Input::GetMouseButtonUp(0)) {
        if (onClick) onClick();
    }
}
