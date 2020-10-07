#include "ScriptableBehaviour.hpp"

void ScriptableBehaviour::update(const sf::Time ts) {
	if (owner == nullptr) return;

	if (!ranStart) {
		onStart();
		ranStart = true;
	}
	onUpdate(ts);
}

std::string ScriptableBehaviour::getIdentifier() const { return identifier; }
void ScriptableBehaviour::setOwner(GameObject* newOwner) { owner = newOwner; }
ScriptableBehaviour::ScriptableBehaviour(const std::string identifier) : identifier(identifier), owner(nullptr) {}
