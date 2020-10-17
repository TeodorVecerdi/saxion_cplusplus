#include "Sprite.hpp"

Sprite::Sprite(const std::string& texturePath) : Sprite(string_format("Sprite_%x", reinterpret_cast<void*>(this)), texturePath) {}

Sprite::Sprite(const std::string& identifier, const std::string& texturePath) : GameObject(identifier) {
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);
}

sf::Sprite* Sprite::getSprite() { return &sprite; }

void Sprite::onStart() {}

void Sprite::onUpdate(sf::Time ts) {
    const auto renderTransform = this->getRenderTransform();
    sprite.setPosition(renderTransform.position.x, renderTransform.position.y);
    sprite.setScale(renderTransform.scale.x, renderTransform.scale.y);
    sprite.setRotation(renderTransform.rotation.z);

    const auto origin = renderTransform.origin;
    const auto spriteRect = sprite.getGlobalBounds();
    sprite.setOrigin(origin.x * spriteRect.width, origin.y * spriteRect.height);
}

void Sprite::onRender(sf::RenderWindow& renderWindow) const {
    renderWindow.draw(sprite);
}
