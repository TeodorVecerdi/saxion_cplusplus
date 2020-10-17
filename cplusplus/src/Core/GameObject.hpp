#pragma once
#include "Core.hpp"
#include "ScriptableBehaviour.hpp"
#include "Transform2D.hpp"
template <typename T> concept behaviour_type = std::is_base_of<ScriptableBehaviour, T>::value;

class GameObject {
public:
    GameObject();
    GameObject(std::string identifier);
    GameObject(const GameObject& other);
    virtual ~GameObject() = default;
    virtual void update(sf::Time ts) final;
    void render(sf::RenderWindow& window) const;
    std::string getIdentifier() const;
public:
    void addScriptableBehaviour(ScriptableBehaviour& behaviour);
    template <behaviour_type T>
    T* getScriptableBehaviour();
    template <behaviour_type T>
    std::vector<T*> getScriptableBehaviours();
    bool removeScriptableBehaviour(ScriptableBehaviour& behaviour);
    bool removeScriptableBehaviour(const std::string& identifier);
    void addChild(GameObject& gameObject);
    bool removeChild(GameObject& gameObject);
    bool removeChild(const std::string& identifier);
public: // TRANSFORM
    void setPosition(glm::vec2 newPosition);
    void movePosition(glm::vec2 deltaPosition);
    glm::vec2 getPosition() const;
    void setScale(glm::vec2 newScale);
    glm::vec2 getScale() const;
    void setOrigin(glm::vec2 newOrigin);
    glm::vec2 getOrigin() const;
    void setRotation(float newRotation);
    float getRotation() const;
    Transform2D getRenderTransform() const;
protected:
    virtual void onStart() {}
    virtual void onRender(sf::RenderWindow& window) const {}
    virtual void onUpdate(const sf::Time ts) {}
    Transform2D transform;
    const std::string identifier;
    GameObject* parent = nullptr;
private:
    bool ranStart;
    std::map<std::string, ScriptableBehaviour*> scriptableBehaviourMap;
    std::vector<ScriptableBehaviour*> scriptableBehaviours;
    std::map<std::string, GameObject*> childrenMap;
    std::vector<GameObject*> children;
};

template <behaviour_type T>
T* GameObject::getScriptableBehaviour() {
    for (auto behaviour : scriptableBehaviours) {
        auto castBehaviour = dynamic_cast<T*>(behaviour);
        if (castBehaviour) return castBehaviour;
    }
    return nullptr;
}

template <behaviour_type T>
std::vector<T*> GameObject::getScriptableBehaviours() {
    std::vector<T*> values;
    for (auto behaviour : scriptableBehaviours) {
        auto castBehaviour = dynamic_cast<T*>(behaviour);
        if (castBehaviour) values.push_back(castBehaviour);
    }
    return values;
}
