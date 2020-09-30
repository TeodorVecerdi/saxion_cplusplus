#include "Sprite.hpp"
Sprite::Sprite(sf::Texture& texture, sf::Vector2f position) : Sprite(texture, position, 1.0f) {}

Sprite::Sprite(sf::Texture& texture, sf::Vector2f position, float speed) : isVisible(true), speed(speed) {
	sprite = sf::Sprite(texture);
	sprite.move(position);

	currentUndoCount = 0;
}

void Sprite::render(sf::RenderWindow& renderWindow) const {
	if (!isVisible) return;
	renderWindow.draw(sprite);
}

void Sprite::update(const sf::Event event) {
	auto moveVector = sf::Vector2f(0.0f, 0.0f);
	if (event.type == sf::Event::KeyPressed) {
		handleMovement(moveVector, event.key.code);
	}
	sprite.move(moveVector);
}

void Sprite::handleMovement(sf::Vector2f& vec, KeyCode eventKey) {
	vec.x = 0.0f;
	vec.y = 0.0f;

	if (eventKey == KeyCode::W || eventKey == KeyCode::Up) vec.y -= speed;
	if (eventKey == KeyCode::S || eventKey == KeyCode::Down) vec.y += speed;
	if (eventKey == KeyCode::A || eventKey == KeyCode::Left) vec.x -= speed;
	if (eventKey == KeyCode::D || eventKey == KeyCode::Right) vec.x += speed;

	if (vec != sf::Vector2f(0, 0)) {
		recordUndo(vec);
	}

	if (eventKey == KeyCode::Space) isVisible = !isVisible;

	if (eventKey == KeyCode::Z) {
		undo();
	}
}

void Sprite::recordUndo(sf::Vector2f undoVec) {
	if (currentUndoCount == 5) {
		// shift array
		for (int i = 1; i < 5; i++) undoMovements[i - 1] = undoMovements[i];
		currentUndoCount--;
	}
	undoMovements[currentUndoCount] = undoVec;
	currentUndoCount++;
}

void Sprite::undo() {
	if (currentUndoCount == 0) return;
	sprite.move(-undoMovements[--currentUndoCount]);
}
