#include "Button.hpp"

#include "Core/FontCache.hpp"
#include "Core/Input.hpp"

Button::Button(const std::string& text, const std::string& fontName, const std::string& buttonTexturePath, void (*onClick)()) : Button(
    string_format("Button_%x", reinterpret_cast<void*>(this)), text, fontName, buttonTexturePath, onClick) {}

Button::Button(const std::string& identifier, const std::string& text, const std::string& fontName, const std::string& buttonTexturePath,
               void (* onClick)()) : GameObject(identifier), onClick(onClick) {
    label.setString(text);
    label.setFont(FontCache::GetFont(fontName));
    label.setFillColor(sf::Color::Black);
    label.setCharacterSize(20);
    texture.loadFromFile(buttonTexturePath);
    sprite.setTexture(texture);
}

void Button::onRender(sf::RenderWindow& window) const {
    window.draw(sprite);
    window.draw(label);
}

void Button::onUpdate(sf::Time ts) {
    const auto renderTransform = this->getRenderTransform();
    const auto spriteSize = sprite.getTexture()->getSize();
    const glm::vec2 sizeMultiplier(size.x / (float)spriteSize.x, size.y / (float)spriteSize.y);

    sprite.setScale(renderTransform.scale.x * sizeMultiplier.x, renderTransform.scale.y * sizeMultiplier.y);
    sprite.setRotation(renderTransform.rotation.z);
    sprite.setPosition(renderTransform.position.x, renderTransform.position.y);

    label.setScale(renderTransform.scale.x, renderTransform.scale.y);
    label.setRotation(renderTransform.rotation.z);
    label.setPosition(renderTransform.position.x, renderTransform.position.y);

    const auto origin = renderTransform.origin;

    const auto spriteRect = sprite.getGlobalBounds();
    const auto labelRect = label.getGlobalBounds();
    sprite.setOrigin(origin.x * spriteSize.x, origin.y * spriteSize.y);
    label.setOrigin(0.5f * labelRect.width, 0.9f * labelRect.height);

    const auto isHovering = spriteRect.contains(Input::mouseX, Input::mouseY);
    if (isHovering && Input::GetMouseButton(0)) {
        sprite.setColor(activeColor);
    } else if (isHovering) {
        sprite.setColor(hoverColor);
    } else {
        sprite.setColor(normalColor);
    }
    if (isHovering && Input::GetMouseButtonUp(0)) onClick();
}

void Button::setSize(glm::vec2 size) {
    this->size = size;
}

glm::vec2 Button::getSize() const {
    return size;
}
