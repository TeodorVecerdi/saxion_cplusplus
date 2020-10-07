#include "OtherPlayerController.hpp"
#include <glm/vec2.hpp>
#include "Core/GameObject.hpp"
#include "Core/Input.hpp"

void OtherPlayerController::onUpdate(const sf::Time ts) {
	glm::vec2 movementVec(0, 0);
	float rotation = 0;

	if (Input::GetKey(sf::Keyboard::Key::Up)) movementVec.y -= 1;
	if (Input::GetKey(sf::Keyboard::Key::Down)) movementVec.y += 1;
	if (Input::GetKey(sf::Keyboard::Key::Left)) movementVec.x -= 1;
	if (Input::GetKey(sf::Keyboard::Key::Right)) movementVec.x += 1;
	if (Input::GetKey(sf::Keyboard::Key::Comma)) rotation += -1;
	if (Input::GetKey(sf::Keyboard::Key::Period)) rotation += 1;

	owner->movePosition(movementVec * (speed * ts.asSeconds()));
	owner->setRotation(owner->getRotation() + rotation * rotationSpeed);
}

OtherPlayerController::OtherPlayerController(const std::string& identifier, float speed, float rotationSpeed) : ScriptableBehaviour(identifier), speed(speed), rotationSpeed(rotationSpeed) {}
