#include "Sprite.hpp"

Sprite::Sprite(const std::string& identifier, const std::string& texturePath) : GameObject(identifier) {
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture);
}

void Sprite::onStart() {
}

void Sprite::onUpdate(sf::Time ts) {
	const auto renderTransform = this->getRenderTransform();
	sprite.setPosition(renderTransform.position.x, renderTransform.position.y);
	sprite.setScale(renderTransform.scale.x, renderTransform.scale.y);
	sprite.setRotation(renderTransform.rotation.z);
	
	const auto origin = renderTransform.origin;
	const auto spriteRect = sprite.getTextureRect();
	sprite.setOrigin(origin.x * spriteRect.width, origin.y * spriteRect.height);

	// printf("Sprite[%s] is at world position [%f, %f, %f]\n", identifier.c_str(), renderTransform.position.x, renderTransform.position.y, renderTransform.position.z);
	
}

void Sprite::onRender(sf::RenderWindow& renderWindow) const {
	renderWindow.draw(sprite);
}