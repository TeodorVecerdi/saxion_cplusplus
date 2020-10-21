#pragma once
#include "Core.hpp"
#include "GameObject.hpp"

class Scene final : public GameObject {
public:
	Scene(std::string identifier);
	~Scene() = default;
	void onStart() override {}
	void onUpdate(sf::Time ts) override {}
	void onRender(sf::RenderWindow& renderWindow) const override {}
	void resetStartTriggers();
public: // Removed methods from GameObject as they don't really make sense in a 'Scene' object
	void addScriptableBehaviour(ScriptableBehaviour& behaviour) = delete;
	bool removeScriptableBehaviour(ScriptableBehaviour& behaviour) = delete;
	bool removeScriptableBehaviour(const std::string& identifier) = delete;
	void setPosition(glm::vec2 newPosition) = delete;
	void movePosition(glm::vec2 deltaPosition) = delete;
	void setScale(glm::vec2 newScale) = delete;
	void setOrigin(glm::vec2 newOrigin) = delete;
	void setRotation(float newRotation) = delete;
};
