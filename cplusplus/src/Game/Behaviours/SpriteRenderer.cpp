#include "SpriteRenderer.hpp"

#include "Core/GameObject.hpp"

SpriteRenderer::SpriteRenderer(const std::string& texturePath, bool setSmooth) : SpriteRenderer(default_identifier(this), texturePath, setSmooth){}

SpriteRenderer::SpriteRenderer(const std::string& identifier, const std::string& texturePath, bool setSmooth) :ScriptableBehaviour(identifier) {
    setTexture(texturePath);
    texture.setSmooth(setSmooth);
    sprite.setTexture(texture);
    const auto textureSize = texture.getSize();
    setSize(glm::vec2(textureSize.x, textureSize.y));
}

void SpriteRenderer::setTexture(const std::string& texturePath) {
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture, true);
    recalculateSizeMultiplier();
}

void SpriteRenderer::setSize(const glm::vec2 size) {
    this->size = size;
    recalculateSizeMultiplier();
}

glm::vec2 SpriteRenderer::getSize() const { return size; }

void SpriteRenderer::setColor(const sf::Color color) {
    sprite.setColor(color);
}

sf::Color SpriteRenderer::getColor() const {
    return sprite.getColor();
}

sf::FloatRect SpriteRenderer::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void SpriteRenderer::recalculateSizeMultiplier() {
    const auto textureSize = texture.getSize();
    this->textureSize = glm::vec2(textureSize.x, textureSize.y);
    sizeMultiplier = size / this->textureSize;
}

void SpriteRenderer::onUpdate(sf::Time ts) {
    const auto renderTransform = owner->getRenderTransform();
    sprite.setScale(renderTransform.scale.x * sizeMultiplier.x, renderTransform.scale.y * sizeMultiplier.y);
    sprite.setRotation(renderTransform.rotation.z);
    sprite.setPosition(renderTransform.position.x, renderTransform.position.y);
    sprite.setOrigin(renderTransform.origin.x * textureSize.x, renderTransform.origin.y * textureSize.y);
}

void SpriteRenderer::onRender(sf::RenderWindow& window) const {
    window.draw(sprite);
}