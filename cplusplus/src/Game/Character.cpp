#include "Character.hpp"

#include <fstream>

#include "Core/IO.hpp"
#include "Utils/StringUtils.hpp"

Character::Character(const std::string& name, const std::string& characterType, int health, int vitality, int attack, int defense, int stamina) : health(health),
    vitality(vitality), attack(attack), defense(defense), stamina(stamina), name(name), characterType(characterType) {}

Character::Character(const std::string& fileName) {
    loadFromFile(fileName);
}

void Character::setName(const std::string& name) {
    this->name = name;
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

void Character::exportCharacter(const std::string& fileName) const {
    if (IO::BeginWrite(fileName)) {
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
        attack = 0;
        defense = 0;
        stamina = 0;
    }
}
