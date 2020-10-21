#include "Scene.hpp"

Scene::Scene(const std::string identifier) :GameObject(identifier) {
    // Setup default transform for a scene
    transform.position = glm::vec3(0);
    transform.origin = glm::vec2(0);
    transform.scale = glm::vec3(1);
    transform.rotation = glm::vec3(0);
    transform.rebuildMatrix();
}

void Scene::resetStartTriggers() {
    for (auto* child : this->children)
        child->resetRanStart();
}
