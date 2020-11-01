#include "SpriteRenderer.hpp"

#include "Core/Scene/GameObject.hpp"

SpriteRenderer::SpriteRenderer(const std::string& texturePath, const bool setSmooth) :
	SpriteRenderer(default_identifier(this), texturePath, setSmooth) {
}

SpriteRenderer::SpriteRenderer(const std::string& identifier, const std::string& texturePath, const bool setSmooth) : ScriptableBehaviour(identifier), smooth(setSmooth), enabled(true) {
	setTexture(texturePath);
	texture.setSmooth(setSmooth);
	sprite.setTexture(texture);
	const auto textureSize = texture.getSize();
	SpriteRenderer::setSize(glm::vec2(textureSize.x, textureSize.y));
}

void SpriteRenderer::setTexture(const std::string& texturePath, const uint32_t spliceBorder) {
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture, true);
	recalculateSizeMultiplier();
}

void SpriteRenderer::setTexture(const sf::Texture* texture, const uint32_t spliceBorder) {
	this->texture = *texture;
	this->texture.setSmooth(smooth);
	sprite.setTexture(*texture, true);
	recalculateSizeMultiplier();
}

void SpriteRenderer::setSize(const glm::vec2 size) {
	this->size = size;
	recalculateSizeMultiplier();
}

void SpriteRenderer::setSmooth(bool smooth) {
	this->smooth = smooth;
	texture.setSmooth(smooth);
	sprite.setTexture(texture, true);
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

void SpriteRenderer::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void SpriteRenderer::recalculateSizeMultiplier() {
	const auto textureSize = texture.getSize();
	this->textureSize = glm::vec2(textureSize.x, textureSize.y);
	sizeMultiplier = size / this->textureSize;
}

void SpriteRenderer::onUpdate(sf::Time ts) {
	if(!enabled) return;
	const auto renderTransform = owner->getRenderTransform();

	sprite.setScale(renderTransform.scale.x * sizeMultiplier.x, renderTransform.scale.y * sizeMultiplier.y);
	sprite.setRotation(renderTransform.rotation.z);
	sprite.setPosition(renderTransform.position.x, renderTransform.position.y);
	sprite.setOrigin(renderTransform.origin.x * textureSize.x, renderTransform.origin.y * textureSize.y);
}

void SpriteRenderer::onRender(sf::RenderWindow& window) const {
	if(!enabled) return;
	window.draw(sprite);
}
