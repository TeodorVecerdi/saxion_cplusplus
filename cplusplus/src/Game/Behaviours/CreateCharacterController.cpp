#include "CreateCharacterController.hpp"

#include "SpriteRenderer.hpp"
#include "TextRenderer.hpp"

CreateCharacterController::CreateCharacterController(int maxPoints, SpriteRenderer* characterImage, TextRenderer* characterNameText, TextRenderer* characterTypeText,
                                                     TextRenderer* vitalityText, TextRenderer* damageText, TextRenderer* defenseText,
                                                     TextRenderer* remainingPointsText) : maxPoints(maxPoints), characterImage(characterImage),
                                                                                          characterNameText(characterNameText), characterTypeText(characterTypeText),
                                                                                          vitalityText(vitalityText), damageText(damageText), defenseText(defenseText),
                                                                                          remainingPointsText(remainingPointsText) {}

CreateCharacterController::CreateCharacterController(const std::string& identifier, int maxPoints, SpriteRenderer* characterImage, TextRenderer* characterNameText,
                                                     TextRenderer* characterTypeText, TextRenderer* vitalityText, TextRenderer* damageText, TextRenderer* defenseText,
                                                     TextRenderer* remainingPointsText): ScriptableBehaviour(identifier), characterImage(characterImage),
                                                                                         characterNameText(characterNameText), characterTypeText(characterTypeText),
                                                                                         vitalityText(vitalityText), damageText(damageText), defenseText(defenseText),
                                                                                         remainingPointsText(remainingPointsText) {}

void CreateCharacterController::changeVitality(int amount) {
    if (remainingPoints == 0 && amount > 0) return;
    vitalityPoints = std::max(vitalityPoints + amount, 1);
    vitalityText->setText(string_format("Vitality: %i", vitalityPoints));
    calculateRemainingPoints();
}

void CreateCharacterController::changeDamage(int amount) {
    if (remainingPoints == 0 && amount > 0) return;
    damagePoints = std::max(damagePoints + amount, 1);
    damageText->setText(string_format("Damage: %i", damagePoints));
    calculateRemainingPoints();
}

void CreateCharacterController::changeDefense(int amount) {
    if (remainingPoints == 0 && amount > 0) return;
    defensePoints = std::max(defensePoints + amount, 1);
    defenseText->setText(string_format("Defense: %i", defensePoints));
    calculateRemainingPoints();
}

void CreateCharacterController::randomizeName() {
    namesIndex = rand() % 60;
    characterNameText->setText(names[namesIndex]);
}

void CreateCharacterController::changeTypeIndex(int amount) {
    typesIndex += amount;
    if (typesIndex < 0) typesIndex += 5;
    typesIndex %= 5;

    auto copy = types[typesIndex];
    std::transform(copy.begin(), copy.end(), copy.begin(), toupper);
    characterTypeText->setText(copy);
    changeTypeImage();
}

void CreateCharacterController::calculateRemainingPoints() {
    remainingPoints = maxPoints - vitalityPoints - damagePoints - defensePoints + 3;
    remainingPointsText->setText(string_format("Remaining points: %i", remainingPoints));
}

void CreateCharacterController::changeTypeImage() {
    characterImage->setTexture(std::string("res/characters/") + types[typesIndex] + "/head_idle.png");
}

void CreateCharacterController::onStart() {
    typesIndex = rand() % 5;
    changeTypeIndex(0);
    randomizeName();
    vitalityPoints = 1;
    damagePoints = 1;
    defensePoints = 1;
    calculateRemainingPoints();
}
