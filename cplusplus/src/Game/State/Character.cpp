#include "Character.hpp"

#include "State/Difficulty.hpp"
#include "Utils/Debug.hpp"

Character::Character(const std::string& name, const std::string& characterType, int vitality, int attack, int defense,
                     int stamina) : health(vitality * healthMultiplier),
                                    vitality(vitality), attack(attack), defense(defense), stamina(stamina), name(name),
                                    characterType(characterType) {
	Character::loadBaseStats();
	isPlayer = false;
}

Character::Character(const std::string& fileName) {
	loadFromFile(fileName);
	Character::loadBaseStats();
	isPlayer = false;
}

Character::Character() {
	Character::loadBaseStats();
	isPlayer = false;
}

void Character::setName(const std::string& name) {
	this->name = name;
}

void Character::setCharacterType(const std::string& characterType) {
	this->characterType = characterType;
}

void Character::setHealth(const int health) {
	this->health = health;
}

void Character::setVitality(const int vitality) {
	this->vitality = vitality;
}

void Character::setAttack(const int attack) {
	this->attack = attack;
}

void Character::setDefense(const int defense) {
	this->defense = defense;
}

void Character::setStamina(const int stamina) {
	this->stamina = stamina;
}

std::string Character::getCharacterType() const {
	return characterType;
}

std::string Character::getName() const {
	return name;
}

int Character::getHealth() const {
	return health;
}

int Character::getMaxHealth() const {
	return vitality * healthMultiplier;
}

int Character::getVitality() const {
	return vitality;
}

int Character::getAttack() const {
	return attack;
}

int Character::getDefense() const {
	return defense;
}

int Character::getStamina() const {
	return stamina;
}

int Character::getAttackPower() const {
	int attackPower = getAttack() + rand() % 5 - 2; // attack +/- 2
	if (attackPower <= 0) attackPower = 1;          // dont heal enemy
	return attackPower;
}

int Character::getDefensePower() const {
	int defensePower = getDefense() + rand() % 5 - 2; // defense +/- 2
	return defensePower;
}

int Character::getHealPower() const {
	int healPower = getStamina() + rand() % 11 - 5; // stamina +/- 5
	if (healPower <= 0) healPower = 1;              // at least heal a bit if you were unlucky
	return healPower;
}

bool Character::takeDamage(const int damage) {
	if (canEvade()) return true;
	health -= damage;
	return false;
}

int Character::heal(int healPower) {
	const int maxHealth = getMaxHealth();
	if (health + healPower > maxHealth) healPower = maxHealth - health;
	health += healPower;
	return healPower;
}

void Character::setIsPlayer(const bool isPlayer) {
	this->isPlayer = isPlayer;
}

bool Character::canEvade() const {
	int chance = Difficulty::GetDifficultySettings().EnemyEvadeChance;
	if (isPlayer) chance = Difficulty::GetDifficultySettings().PlayerEvadeChance;

	bool evade = false;
	for (int i = getDefense(); i > 0; i--) {
		evade |= rand() % 100 < chance;
	}
	return evade;
}

void Character::randomize(int maxPoints) {
	const int enemyTypeIndex = rand() % maxTypes;
	const auto characterType = types[enemyTypeIndex];

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

	ASSERT(vitality + attack + defense + stamina == maxPoints, "vitality + attack + defense + stamina != maxPoints");
	this->characterType = characterType;
	this->vitality = vitality + baseStats[enemyTypeIndex].vitality;
	this->attack = attack + baseStats[enemyTypeIndex].attack;
	this->defense = defense + baseStats[enemyTypeIndex].defense;
	this->stamina = stamina + baseStats[enemyTypeIndex].stamina;
	printf(
		"[Character::randomize(int)] Randomized character with stats vitality=%i, attack=%i, defense=%i, stamina=%i\n",
		vitality, attack,
		defense, stamina);
	this->health = getMaxHealth();
}

void Character::loadFromCharacter(Character& character) {
	setName(character.getName());
	setCharacterType(character.getCharacterType());
	setAttack(character.getAttack());
	setDefense(character.getDefense());
	setHealth(character.getHealth());
	setStamina(character.getStamina());
	setVitality(character.getVitality());
	setIsPlayer(character.isPlayer);
}

void Character::exportCharacter(const std::string& fileName, const int openMode) const {
	if (IO::BeginWrite(fileName, openMode)) {
		IO::WriteTitle(string_format("###### CHARACTER [%s]_[%s]", capitalize(name).c_str(),
		                             capitalize(characterType).c_str()));
		IO::Write(name, "name");
		IO::Write(characterType, "characterType");
		IO::Write(health, "health");
		IO::Write(vitality, "vitality");
		IO::Write(attack, "attack");
		IO::Write(defense, "defense");
		IO::Write(stamina, "stamina");
		IO::EndWrite();
	}
}

void Character::loadFromFile(const std::string& fileName) {
	if (IO::BeginRead(fileName)) {
		IO::ReadString(name);
		IO::ReadString(characterType);
		IO::ReadInt(health);
		IO::ReadInt(vitality);
		IO::ReadInt(attack);
		IO::ReadInt(defense);
		IO::ReadInt(stamina);
		IO::EndRead();
	} else {
		printf("Failed to load file %s. Check that the file exists.\n", fileName.c_str());
		printf("Initializing blank character with default stats.\n");
		name = default_identifier(this);
		characterType = "player";
		health = 0;
		vitality = 0;
		attack = 0;
		defense = 0;
		stamina = 0;
	}
}

void Character::loadBaseStats() {
	if (Character::loadedBaseStats) return;
	Character::loadedBaseStats = true;
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
