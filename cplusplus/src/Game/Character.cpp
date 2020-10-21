#include "Character.hpp"

#include <fstream>


#include "Core/IO.hpp"
#include "Utils/StringUtils.hpp"

Character::Character(const std::string& name, const std::string& characterType, int health, int vitality, int attack, int defense) : health(health), vitality(vitality), attack(attack), defense(defense), name(name),
                                                                                                                   characterType(characterType) {}

Character::Character(const std::string& fileName) {
    loadFromFile(fileName);
}

void Character::exportCharacter(const std::string& fileName) {
    if(IO::BeginWrite(fileName)) {
        IO::Write(name, "name");
        IO::Write(characterType, "characterType");
        IO::Write(health, "health");
        IO::Write(vitality, "vitality");
        IO::Write(attack, "attack");
        IO::Write(defense, "defense");
        IO::EndWrite();
    }
}

void Character::setName(const std::string& name) {
    this->name = name;
}

void Character::setHealth(int health) {
    this->health = health;
}

void Character::setVitality(int vitality) {
    this->vitality = vitality;
}


void Character::setAttack(int attack) {
    this->attack = attack;
}

void Character::setDefense(int defense) {
    this->defense = defense;
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

void Character::loadFromFile(const std::string& fileName) {
    if(IO::BeginRead(fileName)) {
        IO::ReadString(name);
        IO::ReadString(characterType);
        IO::ReadInt(health);
        IO::ReadInt(vitality);
        IO::ReadInt(attack);
        IO::ReadInt(defense);
        IO::EndRead();
    } else {
        printf("Failed to load file %s. Check that the file exists.\n", fileName.c_str());
        printf("Initializing blank character with default stats.\n");
        name = default_identifier(this);
        characterType = "player";
        health = 0;
        attack = 0;
        defense = 0;
    }
}
