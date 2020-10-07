#include "GameObject.hpp"
#include "Utils/Debug.hpp"
#include <glm/gtc/matrix_transform.hpp>
GameObject::GameObject(const std::string identifier) : transform(Transform2D(glm::vec2(0, 0), glm::vec2(1, 1), glm::vec2(.5f, .5f), glm::vec3(0))), identifier(identifier), ranStart(false) {}
GameObject::GameObject(const GameObject& other) : transform(other.transform), identifier(other.identifier), ranStart(false) {}

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
	ASSERT(!scriptableBehaviours.contains(id), "Cannot add a ScriptableBehaviour with identifier [" << id << "] when another one with the same identifier already exists.");
	behaviour.setOwner(this);
	scriptableBehaviours.emplace(std::make_pair(id, &behaviour));
}

bool GameObject::removeScriptableBehaviour(ScriptableBehaviour& behaviour) {
	return this->removeScriptableBehaviour(behaviour.getIdentifier());
}

bool GameObject::removeScriptableBehaviour(const std::string& identifier) {
	if (!scriptableBehaviours.contains(identifier)) {
		printf("GameObject[%s] does not contain a scriptable behaviour with identifier [%s]", this->identifier.c_str(), identifier.c_str());
		return false;
	}
	scriptableBehaviours.at(identifier)->setOwner(nullptr);
	scriptableBehaviours.erase(identifier);
	return true;
}

void GameObject::addChild(GameObject& gameObject) {
	std::string id = gameObject.getIdentifier();
	ASSERT(!children.contains(id), "Cannot add a child GameObject with identifier [" << id << "] when another one with the same identifier already exists.");
	gameObject.parent = this;
	children.emplace(std::make_pair(id, &gameObject));
}

bool GameObject::removeChild(GameObject& gameObject) {
	return this->removeChild(gameObject.getIdentifier());
}

bool GameObject::removeChild(const std::string& identifier) {
	if (!children.contains(identifier)) {
		printf("GameObject[%s] does not contain a child with identifier [%s]", this->identifier.c_str(), identifier.c_str());
		return false;
	}
	children.at(identifier)->parent = nullptr;
	children.erase(identifier);
	return true;
}

void GameObject::setPosition(const glm::vec2 newPosition) {
	transform.position = glm::vec3(newPosition.x, newPosition.y, 0);
	transform.rebuildMatrix();
}

void GameObject::movePosition(const glm::vec2 deltaPosition) {
	transform.position += glm::vec3(deltaPosition.x, deltaPosition.y, 0);
	transform.rebuildMatrix();
}

glm::vec2 GameObject::getPosition() const {
	return glm::vec2(transform.position.x, transform.position.y);
}

void GameObject::setScale(const glm::vec2 newScale) {
	transform.scale = glm::vec3(newScale.x, newScale.y, 1);
	transform.rebuildMatrix();
}

glm::vec2 GameObject::getScale() const {
	return glm::vec2(transform.scale.x, transform.scale.y);
}

void GameObject::setOrigin(glm::vec2 newOrigin) {
	transform.origin = newOrigin;
}

glm::vec2 GameObject::getOrigin() const {
	return transform.origin;
}

void GameObject::setRotation(float newRotation) {
	transform.rotation.z = newRotation;
	transform.rebuildMatrix();
}

float GameObject::getRotation() const {
	return transform.rotation.z;
}

Transform2D GameObject::getRenderTransform() const {
	if (parent == nullptr) {
		return transform;
	}

	auto current = transform.localToWorld;
	auto* currentParent = parent;
	while (currentParent != nullptr) {
		current = currentParent->transform.localToWorld * current;
		currentParent = currentParent->parent;
	}
	Transform2D newTransform(glm::vec2(0), glm::vec2(0), transform.origin, glm::vec3(0));
	newTransform.setPRS(current);
	return newTransform;
}
