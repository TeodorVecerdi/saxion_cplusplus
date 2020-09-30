#pragma once
#include <array>
#include <SFML/Graphics.hpp>

typedef sf::Keyboard::Key KeyCode;

class Sprite {
public:
	Sprite(sf::Texture& texture, sf::Vector2f position);
	Sprite(sf::Texture& texture, sf::Vector2f position, float speed);
	~Sprite() = default;
	void render(sf::RenderWindow& renderWindow) const;
	void update(sf::Event event);
private:
	void handleMovement(sf::Vector2f& vec, KeyCode eventKey);
	void recordUndo(sf::Vector2f undoVec);
	void undo();
private:
	sf::Sprite sprite;
	bool isVisible;
	float speed;
	int currentUndoCount;
	std::array<sf::Vector2f, 5> undoMovements;
};
