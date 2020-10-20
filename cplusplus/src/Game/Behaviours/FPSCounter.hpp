#pragma once
#include "Core/ScriptableBehaviour.hpp"
class TextRenderer;

class FPSCounter final : public ScriptableBehaviour {
public:
    FPSCounter(const std::string& identifier, float updateDelay) : ScriptableBehaviour(identifier), updateDelay(updateDelay) {}

protected:
    void onStart() override;
    void onUpdate(sf::Time ts) override;
private:
    float updateDelay;
    float updateDelayTimer;

    float minFPS;
    float maxFPS;
    float currentFPS;
    TextRenderer* textRenderer;
};
