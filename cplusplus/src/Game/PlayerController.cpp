#include "PlayerController.hpp"
#include <SFML/Window/Keyboard.hpp>

#include "Core/Input.hpp"

void PlayerController::onUpdate(const sf::Time ts) {

	sf::Vector2f movementVec(0, 0);
	
	if(Input::GetKey(sf::Keyboard::Key::W) || Input::GetKey(sf::Keyboard::Key::Up)) movementVec.y -= 1;
	if(Input::GetKey(sf::Keyboard::Key::S) || Input::GetKey(sf::Keyboard::Key::Down)) movementVec.y += 1;
	if(Input::GetKey(sf::Keyboard::Key::A) || Input::GetKey(sf::Keyboard::Key::Left)) movementVec.x -= 1;
	if(Input::GetKey(sf::Keyboard::Key::D) || Input::GetKey(sf::Keyboard::Key::Right)) movementVec.x += 1;

	sprite.movePosition(movementVec * (speed * ts.asSeconds()));
}

void PlayerController::onStart() {
	printf("PlayerController[%s]::onStart called", getIdentifier().c_str());
}
PlayerController::PlayerController(const std::string& identifier, const float speed, Sprite& sprite) : ScriptableBehaviour(identifier), speed(speed), sprite(sprite) {}
