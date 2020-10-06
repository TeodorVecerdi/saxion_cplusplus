#include "ScriptableBehaviour.hpp"

void ScriptableBehaviour::update(const sf::Time ts) {
	if (!ranStart) {
		onStart();
		ranStart = true;
	}
	onUpdate(ts);
}

std::string ScriptableBehaviour::getIdentifier() const { return identifier; }
ScriptableBehaviour::ScriptableBehaviour(const std::string identifier) : identifier(identifier) {}
