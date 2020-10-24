#include "FPSCounter.hpp"
#include "Core/Scene/GameObject.hpp"

void FPSCounter::onStart() {
    updateDelayTimer = updateDelay;
}

void FPSCounter::onUpdate(const sf::Time ts) {
    updateDelayTimer += ts.asSeconds();
    if(updateDelayTimer >= updateDelay) {
        updateDelayTimer -= updateDelay;
        frameTime = ts.asSeconds();
    }
}

void FPSCounter::onRender(sf::RenderWindow& window) const {
    window.setTitle(string_format("Assignment | dT: %fs, %fms", frameTime, frameTime * 1000));
}


