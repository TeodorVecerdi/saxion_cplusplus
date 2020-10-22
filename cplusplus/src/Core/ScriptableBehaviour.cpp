#include "ScriptableBehaviour.hpp"

#include "GameObject.hpp"

void ScriptableBehaviour::update(const sf::Time ts) {
	if (owner == nullptr) return;

	if (!ranStart) {
        ranStart = true;
        onStart();
    }
	onUpdate(ts);
}

void ScriptableBehaviour::render(sf::RenderWindow& window) const {
	if (owner == nullptr) return;
	onRender(window);
}

std::string ScriptableBehaviour::getIdentifier() const { return identifier; }
void ScriptableBehaviour::setOwner(GameObject* newOwner) { owner = newOwner; }

void ScriptableBehaviour::resetRanStart() {
	ranStart = false;
}

void ScriptableBehaviour::destroy() {
	isDestroyed = true;
}

ScriptableBehaviour::ScriptableBehaviour() : ScriptableBehaviour(default_identifier(this)) {}

ScriptableBehaviour::ScriptableBehaviour(const std::string identifier) : identifier(identifier), owner(nullptr), ranStart(false) {}

ScriptableBehaviour::~ScriptableBehaviour() {
	printf("%*s dtor called on behaviour %s\n", GameObject::indent * 2, "", identifier.c_str());
}
