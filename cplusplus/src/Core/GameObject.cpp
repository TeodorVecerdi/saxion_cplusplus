#include "GameObject.hpp"
#include "Utils\Debug.hpp"

GameObject::GameObject(std::string identifier) : identifier(std::move(identifier)), ranStart(false) {}
GameObject::GameObject(const GameObject& other) : identifier(other.getIdentifier()), ranStart(false) {}

void GameObject::update(const sf::Time ts) {
	if (!ranStart) {
		ranStart = true;
		onStart();
	}
	for (auto [id, behaviour] : scriptableBehaviours) {
		behaviour->update(ts);
	}
	onUpdate(ts);
}

void GameObject::render(sf::RenderWindow& window) const {
	onRender(window);
}

std::string GameObject::getIdentifier() const {
	return this->identifier;
}

void GameObject::addScriptableBehaviour(ScriptableBehaviour& behaviour) {
	auto id = behaviour.getIdentifier();
	ASSERT(!scriptableBehaviours.contains(id), "Cannot add a ScriptableBehaviour with identifier" << id << " when another one with the same identifier already exists.");
	scriptableBehaviours.emplace(id, &behaviour);
}
