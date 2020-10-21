#pragma once
#include "Core/ScriptableBehaviour.hpp"
class TextRenderer;

class FPSCounter final : public ScriptableBehaviour {
public:
    FPSCounter(const std::string& identifier, float updateDelay) : ScriptableBehaviour(identifier), updateDelay(updateDelay) {}

protected:
    void onStart() override;
    void onUpdate(sf::Time ts) override;
    void onRender(sf::RenderWindow& window) const override;
private:
    float frameTime;
    float updateDelay;
    float updateDelayTimer;
};
