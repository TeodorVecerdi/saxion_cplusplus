#pragma once
#include "Core.hpp"
#include "Scene.hpp"

class SceneManager {
public:
    SceneManager();
    ~SceneManager() = default;

    void registerScene(Scene& scene);
    bool unregisterScene(const std::string& sceneIdentifier);
    bool unregisterScene(Scene& scene);
    bool stackScene(const std::string& sceneIdentifier, bool resetStart);
    bool stackScene(Scene& scene, bool resetStart);
    Scene& popScene();

    void update(sf::Time ts);
    void render(sf::RenderWindow& renderWindow) const;
private:
    std::map<std::string, Scene*> registeredScenes;
    std::stack<Scene*> sceneStack;
};
