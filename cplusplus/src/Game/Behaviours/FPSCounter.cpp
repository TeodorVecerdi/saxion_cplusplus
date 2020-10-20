#include "FPSCounter.hpp"
#include "TextRenderer.hpp"
#include "Core/GameObject.hpp"

void FPSCounter::onStart() {
    updateDelayTimer = updateDelay;
    minFPS = 100000000.0f;
    maxFPS = 0.0f;
    currentFPS = 0.0f;
    textRenderer = owner->getScriptableBehaviour<TextRenderer>();
}

void FPSCounter::onUpdate(const sf::Time ts) {

    currentFPS = 1.0f / ts.asSeconds();
    if (currentFPS < minFPS) minFPS = currentFPS;
    if (currentFPS > maxFPS) maxFPS = currentFPS;

    updateDelayTimer += ts.asSeconds();
    if(updateDelayTimer >= updateDelay) {
        updateDelayTimer -= updateDelay;
        textRenderer->setText(string_format("AVG: %-10.2f\tMIN: %-10.2f\tMAX: %-10.2f", currentFPS, minFPS, maxFPS));
    }
}
