#include "PlayerController.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Core/Input.hpp"

void PlayerController::onUpdate(const sf::Time ts) {

	glm::vec2 movementVec(0, 0);
	float rotation = 0;

	if (Input::GetKey(sf::Keyboard::Key::W)) movementVec.y -= 1;
	if (Input::GetKey(sf::Keyboard::Key::S)) movementVec.y += 1;
	if (Input::GetKey(sf::Keyboard::Key::A)) movementVec.x -= 1;
	if (Input::GetKey(sf::Keyboard::Key::D)) movementVec.x += 1;

	if (Input::GetKey(sf::Keyboard::Key::Q)) rotation += -1;
	if (Input::GetKey(sf::Keyboard::Key::E)) rotation += 1;

	owner->movePosition(movementVec * (speed * ts.asSeconds()));
	owner->setRotation(owner->getRotation() + rotation * rotationSpeed);
}

PlayerController::PlayerController(const std::string& identifier, const float speed, float rotationSpeed) : ScriptableBehaviour(identifier), speed(speed), rotationSpeed(rotationSpeed) {}
