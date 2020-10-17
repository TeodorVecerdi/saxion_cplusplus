#include "OtherPlayerController.hpp"
#include <glm/vec2.hpp>

#include "Core/GameObject.hpp"
#include "Core/Input.hpp"
#include "Game/Button.hpp"

void OtherPlayerController::onUpdate(const sf::Time ts) {
    auto* button = dynamic_cast<Button*>(owner);
    glm::vec2 movementVec(0, 0);

    if (Input::GetKey(sf::Keyboard::Key::Up)) movementVec.y += 1;
    if (Input::GetKey(sf::Keyboard::Key::Down)) movementVec.y -= 1;
    if (Input::GetKey(sf::Keyboard::Key::Left)) movementVec.x -= 1;
    if (Input::GetKey(sf::Keyboard::Key::Right)) movementVec.x += 1;

    button->setSize(button->getSize() + movementVec);
}

OtherPlayerController::OtherPlayerController(const std::string& identifier, float speed) : ScriptableBehaviour(identifier), speed(speed) {}
