#include "Button.hpp"
#include "SpriteRenderer.hpp"
#include "Core/Input.hpp"

Button::Button(SpriteRenderer& sprite) : ScriptableBehaviour(default_identifier(this)) {
    setSprite(&sprite);
    label = nullptr;
    this->onClick = nullptr;
}

Button::Button(SpriteRenderer& sprite, std::function<void()> onClick) : Button(default_identifier(this), sprite, onClick) {}

Button::Button(const std::string& identifier, SpriteRenderer& sprite, std::function<void()> onClick) : ScriptableBehaviour(identifier) {
    setSprite(&sprite);
    label = nullptr;
    this->onClick = onClick;
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

void Button::onUpdate(const sf::Time ts) {
    const auto spriteRect = sprite->getGlobalBounds();

    const bool isHovering = spriteRect.contains(Input::mouseX, Input::mouseY);

    if (isHovering && Input::GetMouseButton(0)) {
        sprite->setColor(activeColor);
    } else if (isHovering) {
        sprite->setColor(hoverColor);
    } else {
        sprite->setColor(normalColor);
    }
    if (isHovering && Input::GetMouseButtonUp(0)) {
        if (onClick) onClick();
    }
}
