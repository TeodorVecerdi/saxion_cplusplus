#include "ScriptableBehaviour.hpp"

void ScriptableBehaviour::update(const sf::Time ts) {
	if (owner == nullptr) return;

	if (!ranStart) {
		onStart();
		ranStart = true;
	}
	onUpdate(ts);
}

void ScriptableBehaviour::render(sf::RenderWindow& window) const {
	if (owner == nullptr) return;
	onRender(window);
}

std::string ScriptableBehaviour::getIdentifier() const { return identifier; }
void ScriptableBehaviour::setOwner(GameObject* newOwner) { owner = newOwner; }

ScriptableBehaviour::ScriptableBehaviour() : ScriptableBehaviour(default_identifier(this)) {}

ScriptableBehaviour::ScriptableBehaviour(const std::string identifier) : identifier(identifier), owner(nullptr) {}
