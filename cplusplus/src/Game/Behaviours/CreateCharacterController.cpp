#include "CreateCharacterController.hpp"

#include "Core/IO.hpp"
#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Game/State/Character.hpp"
#include "Game/State/Difficulty.hpp"

CreateCharacterController::CreateCharacterController(SpriteRenderer* characterImage, TextRenderer* characterNameText,
                                                     TextRenderer* characterTypeText,
                                                     TextRenderer* vitalityText, TextRenderer* attackText,
                                                     TextRenderer* defenseText, TextRenderer* staminaText,
                                                     TextRenderer* remainingPointsText) :
	characterImage(characterImage),
	characterNameText(characterNameText), characterTypeText(characterTypeText),
	remainingPointsText(remainingPointsText), vitalityText(vitalityText),
	attackText(attackText), defenseText(defenseText), staminaText(staminaText) {
}

CreateCharacterController::CreateCharacterController(const std::string& identifier, SpriteRenderer* characterImage,
                                                     TextRenderer* characterNameText,
                                                     TextRenderer* characterTypeText, TextRenderer* vitalityText,
                                                     TextRenderer* damageText, TextRenderer* defenseText,
                                                     TextRenderer* staminaText, TextRenderer* remainingPointsText):
	ScriptableBehaviour(identifier), characterImage(characterImage),
	characterNameText(characterNameText),
	characterTypeText(characterTypeText),
	remainingPointsText(remainingPointsText),
	vitalityText(vitalityText), attackText(damageText),
	defenseText(defenseText), staminaText(staminaText) {
}

void CreateCharacterController::changeVitality(int amount) {
	if (remainingPoints == 0 && amount > 0) return;
	vitalityPoints = std::max(vitalityPoints + amount, baseStats[typesIndex].vitality);
	vitalityText->setText(string_format("Vitality: %i", vitalityPoints));
	calculateRemainingPoints();
}

void CreateCharacterController::changeAttack(int amount) {
	if (remainingPoints == 0 && amount > 0) return;
	attackPoints = std::max(attackPoints + amount, baseStats[typesIndex].attack);
	attackText->setText(string_format("Attack: %i", attackPoints));
	calculateRemainingPoints();
}

void CreateCharacterController::changeDefense(int amount) {
	if (remainingPoints == 0 && amount > 0) return;
	defensePoints = std::max(defensePoints + amount, baseStats[typesIndex].defense);
	defenseText->setText(string_format("Defense: %i", defensePoints));
	calculateRemainingPoints();
}

void CreateCharacterController::changeStamina(int amount) {
	if (remainingPoints == 0 && amount > 0) return;
	staminaPoints = std::max(staminaPoints + amount, baseStats[typesIndex].stamina);
	staminaText->setText(string_format("Stamina: %i", staminaPoints));
	calculateRemainingPoints();
}

void CreateCharacterController::randomizeName() {
	namesIndex = rand() % 60;
	characterNameText->setText(string_format("Name: %s", names[namesIndex].c_str()));
}

void CreateCharacterController::randomizeStats() {
	int vitality = 0;
	int attack = 0;
	int defense = 0;
	int stamina = 0;

	if (maxPoints < 5) {
		int remaining = maxPoints;
		vitality = rand() % remaining + 1;
		remaining -= vitality;
		if (remaining == 0) attack = defense = stamina = 0;
		else {
			attack = rand() % remaining + 1;
			remaining -= attack;
			if (remaining == 0) defense = stamina = 0;
			else {
				defense = rand() % remaining + 1;
				remaining -= defense;
				if (remaining == 0) stamina = 0;
				else stamina = remaining;
			}
		}
	} else {
		const int mid = rand() % (maxPoints - 2) + 2;           // 2->max
		const int left = rand() % (mid - 1) + 1;                // 2->mid-1
		const int right = mid + rand() % (maxPoints - mid) + 1; //mid+1->max

		vitality = left;
		attack = mid - left;
		defense = right - mid;
		stamina = maxPoints - right;
	}

	vitalityPoints =vitality + baseStats[typesIndex].vitality;
	attackPoints =attack + baseStats[typesIndex].attack;
	defensePoints =defense + baseStats[typesIndex].defense;
	staminaPoints =stamina + baseStats[typesIndex].stamina;

	changeVitality(0); // hack to force UI to update
	changeAttack(0);   // hack to force UI to update
	changeDefense(0);  // hack to force UI to update
	changeStamina(0);  // hack to force UI to update
	calculateRemainingPoints();
}

void CreateCharacterController::changeTypeIndex(int amount) {
	typesIndex += amount;
	if (typesIndex < 0) typesIndex += maxTypes;
	typesIndex %= maxTypes;

	characterTypeText->setText(uppercase(types[typesIndex]));

	vitalityPoints = baseStats[typesIndex].vitality;
	attackPoints = baseStats[typesIndex].attack;
	defensePoints = baseStats[typesIndex].defense;
	staminaPoints = baseStats[typesIndex].stamina;
	changeVitality(0); // hack to force UI to update
	changeAttack(0);   // hack to force UI to update
	changeDefense(0);  // hack to force UI to update
	changeStamina(0);  // hack to force UI to update
	calculateRemainingPoints();

	changeTypeImage();
}

void CreateCharacterController::exportCharacter() const {
	auto fileName = string_format("assets/temp/temp_create_character_%s_%s.txt", names[namesIndex].c_str(),
	                              types[typesIndex].c_str());
	if (IO::BeginWrite(fileName)) {
		IO::Write(names[namesIndex], "name");
		IO::Write(types[typesIndex], "characterType");
		IO::Write(vitalityPoints * Character::healthMultiplier, "health");
		IO::Write(vitalityPoints, "vitality");
		IO::Write(attackPoints, "attack");
		IO::Write(defensePoints, "defense");
		IO::Write(staminaPoints, "stamina");
		IO::EndWrite();
	}
}

std::string CreateCharacterController::getExportPath() const {
	return string_format("assets/temp/temp_create_character_%s_%s.txt", names[namesIndex].c_str(),
	                     types[typesIndex].c_str());
}

void CreateCharacterController::calculateRemainingPoints() {
	remainingPoints = maxPoints - vitalityPoints - attackPoints - defensePoints - staminaPoints + baseStats[typesIndex].
		vitality + baseStats[typesIndex].attack + baseStats
		[typesIndex].defense + baseStats[typesIndex].stamina;
	remainingPointsText->setText(string_format("Remaining points: %i", remainingPoints));
}

void CreateCharacterController::changeTypeImage() {
	characterImage->setTexture(string_format("assets/textures/characters/%s/head_idle.png", types[typesIndex].c_str()));
}

void CreateCharacterController::loadBaseStats() {
	for (int i = 0; i < maxTypes; i++) {
		if (IO::BeginRead(string_format("assets/data/base_character_stats/%s.txt", types[i].c_str()))) {
			IO::ReadInt(baseStats[i].vitality);
			IO::ReadInt(baseStats[i].attack);
			IO::ReadInt(baseStats[i].defense);
			IO::ReadInt(baseStats[i].stamina);
			IO::EndRead();
		}
	}
}

void CreateCharacterController::onStart() {
	maxPoints = Difficulty::GetDifficultySettings().PlayerBaseAttrPoints;
	loadBaseStats();
	typesIndex = rand() % maxTypes;
	changeTypeIndex(0);
	randomizeName();
	calculateRemainingPoints();
}
