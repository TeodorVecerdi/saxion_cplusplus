#include "Sprite.hpp"

Sprite::Sprite(const std::string& identifier, const std::string texturePath) : GameObject(identifier), texturePath(texturePath) {
	this->texture.loadFromFile(this->texturePath);
	this->sprite = sf::Sprite(this->texture);
}

void Sprite::onStart() {
	printf("Sprite[%s]::onStart called", getIdentifier().c_str());
}

void Sprite::onRender(sf::RenderWindow& renderWindow) const {
	renderWindow.draw(sprite);
}

void Sprite::onUpdate(const sf::Time ts) {}

void Sprite::setPosition(const sf::Vector2f& newPosition) {
	position = newPosition;
	sprite.setPosition(position);
}

void Sprite::movePosition(const sf::Vector2f& deltaPosition) {
	position += deltaPosition;
	sprite.setPosition(position);
}

sf::Vector2f Sprite::getPosition() const {
	return position;
}
