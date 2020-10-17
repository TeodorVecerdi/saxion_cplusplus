#include "SceneManager.hpp"

SceneManager::SceneManager() {}

void SceneManager::update(const sf::Time ts) {
    if (!sceneStack.empty())
        sceneStack.top()->update(ts);
}

void SceneManager::render(sf::RenderWindow& renderWindow) const {
    if (!sceneStack.empty())
        sceneStack.top()->render(renderWindow);
}

void SceneManager::registerScene(Scene& scene) {
    auto id = scene.getIdentifier();
    ASSERT(!registeredScenes.contains(id), "Cannot register scene with identifier [" << id << "] when another one with the same identifier already exists");
    registeredScenes.emplace(std::make_pair(id, &scene));
}

bool SceneManager::unregisterScene(const std::string& sceneIdentifier) {
    if(!registeredScenes.contains(sceneIdentifier)) {
        printf("SceneManager does not contain a registered scene with identifier [%s]\n", sceneIdentifier.c_str());
        return false;
    }
    registeredScenes.erase(sceneIdentifier);
    return true;
}

bool SceneManager::unregisterScene(Scene& scene) {
    return this->unregisterScene(scene.getIdentifier());
}

bool SceneManager::stackScene(const std::string& sceneIdentifier) {
    if (!registeredScenes.contains(sceneIdentifier)) {
        printf("SceneManager does not contain a registered scene with identifier [%s]\n", sceneIdentifier.c_str());
        return false;
    }
    sceneStack.push(registeredScenes[sceneIdentifier]);
    return true;
}

bool SceneManager::stackScene(Scene& scene) {
    const auto id = scene.getIdentifier();
    if (!registeredScenes.contains(id)) {
        printf("SceneManager does not contain a registered scene with identifier [%s]\n", id.c_str());
        return false;
    }
    sceneStack.push(&scene);
    return true;
}

Scene& SceneManager::popScene() {
    auto* top = sceneStack.top();
    sceneStack.pop();
    return *top;
}

