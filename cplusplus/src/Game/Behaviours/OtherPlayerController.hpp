#pragma once
#include "Core/ScriptableBehaviour.hpp"

class OtherPlayerController : public ScriptableBehaviour {
public:
    OtherPlayerController(const std::string& identifier, float speed);
protected:
    void onUpdate(sf::Time ts) override;

private:
    float speed;
};
