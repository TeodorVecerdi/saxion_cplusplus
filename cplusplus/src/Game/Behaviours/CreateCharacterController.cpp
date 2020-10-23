#include "CreateCharacterController.hpp"

#include "Core/IO.hpp"
#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"

CreateCharacterController::CreateCharacterController(int maxPoints, SpriteRenderer* characterImage, TextRenderer* characterNameText, TextRenderer* characterTypeText,
                                                     TextRenderer* vitalityText, TextRenderer* damageText, TextRenderer* defenseText, TextRenderer* staminaText,
                                                     TextRenderer* remainingPointsText) : maxPoints(maxPoints), characterImage(characterImage),
                                                                                          characterNameText(characterNameText), characterTypeText(characterTypeText),
                                                                                          remainingPointsText(remainingPointsText), vitalityText(vitalityText),
                                                                                          damageText(damageText), defenseText(defenseText), staminaText(staminaText) {}

CreateCharacterController::CreateCharacterController(const std::string& identifier, int maxPoints, SpriteRenderer* characterImage, TextRenderer* characterNameText,
                                                     TextRenderer* characterTypeText, TextRenderer* vitalityText, TextRenderer* damageText, TextRenderer* defenseText,
                                                     TextRenderer* staminaText, TextRenderer* remainingPointsText): ScriptableBehaviour(identifier), characterImage(characterImage),
                                                                                                                    characterNameText(characterNameText),
                                                                                                                    characterTypeText(characterTypeText),
                                                                                                                    remainingPointsText(remainingPointsText),
                                                                                                                    vitalityText(vitalityText), damageText(damageText),
                                                                                                                    defenseText(defenseText), staminaText(staminaText) {}

void CreateCharacterController::changeVitality(int amount) {
    if (remainingPoints == 0 && amount > 0) return;
    vitalityPoints = std::max(vitalityPoints + amount, defaultStats[typesIndex].vitality);
    vitalityText->setText(string_format("Vitality: %i", vitalityPoints));
    calculateRemainingPoints();
}

void CreateCharacterController::changeDamage(int amount) {
    if (remainingPoints == 0 && amount > 0) return;
    damagePoints = std::max(damagePoints + amount, defaultStats[typesIndex].damage);
    damageText->setText(string_format("Damage: %i", damagePoints));
    calculateRemainingPoints();
}

void CreateCharacterController::changeDefense(int amount) {
    if (remainingPoints == 0 && amount > 0) return;
    defensePoints = std::max(defensePoints + amount, defaultStats[typesIndex].defense);
    defenseText->setText(string_format("Defense: %i", defensePoints));
    calculateRemainingPoints();
}

void CreateCharacterController::changeStamina(int amount) {
    if (remainingPoints == 0 && amount > 0) return;
    staminaPoints = std::max(staminaPoints + amount, defaultStats[typesIndex].stamina);
    staminaText->setText(string_format("Stamina: %i", staminaPoints));
    calculateRemainingPoints();
}

void CreateCharacterController::randomizeName() {
    namesIndex = rand() % 60;
    characterNameText->setText(string_format("Name: %s", names[namesIndex].c_str()));
}

void CreateCharacterController::changeTypeIndex(int amount) {
    typesIndex += amount;
    if (typesIndex < 0) typesIndex += maxTypes;
    typesIndex %= maxTypes;

    characterTypeText->setText(uppercase(types[typesIndex]));

    vitalityPoints = defaultStats[typesIndex].vitality;
    damagePoints = defaultStats[typesIndex].damage;
    defensePoints = defaultStats[typesIndex].defense;
    staminaPoints = defaultStats[typesIndex].stamina;
    changeVitality(0); // hack to force UI to update
    changeDamage(0); // hack to force UI to update
    changeDefense(0); // hack to force UI to update
    changeStamina(0); // hack to force UI to update
    calculateRemainingPoints();

    changeTypeImage();
}

void CreateCharacterController::exportCharacter() const {
    auto fileName = string_format("assets/data/temp_create_character_%s_%s.txt", names[namesIndex].c_str(), types[typesIndex].c_str());
    if(IO::BeginWrite(fileName)) {
        IO::Write(names[namesIndex], "name");
        IO::Write(types[typesIndex], "characterType");
        IO::Write(vitalityPoints*10, "health");
        IO::Write(vitalityPoints, "vitality");
        IO::Write(damagePoints, "attack");
        IO::Write(defensePoints, "defense");
        IO::Write(staminaPoints, "stamina");
        IO::EndWrite();
    }
}

std::string CreateCharacterController::getExportPath() const {
    return string_format("assets/data/temp_create_character_%s_%s.txt", names[namesIndex].c_str(), types[typesIndex].c_str());
}

void CreateCharacterController::calculateRemainingPoints() {
    remainingPoints = maxPoints - vitalityPoints - damagePoints - defensePoints - staminaPoints + defaultStats[typesIndex].vitality + defaultStats[typesIndex].damage + defaultStats
        [typesIndex].defense + defaultStats[typesIndex].stamina;
    remainingPointsText->setText(string_format("Remaining points: %i", remainingPoints));
}

void CreateCharacterController::changeTypeImage() {
    characterImage->setTexture(string_format("assets/textures/characters/%s/head_idle.png", types[typesIndex].c_str()));
}

void CreateCharacterController::readDefaultStats() {
    for (int i = 0; i < maxTypes; i++) {
        if (IO::BeginRead(string_format("assets/data/base_character_stats/%s.txt", types[i].c_str()))) {
            IO::ReadInt(defaultStats[i].vitality);
            IO::ReadInt(defaultStats[i].damage);
            IO::ReadInt(defaultStats[i].defense);
            IO::ReadInt(defaultStats[i].stamina);
            IO::EndRead();
        }
    }
}

void CreateCharacterController::onStart() {
    readDefaultStats();
    typesIndex = rand() % maxTypes;
    changeTypeIndex(0);
    randomizeName();
    calculateRemainingPoints();
}
