#pragma once
#include <string>

class Character {
public:
	Character(const std::string& name, const std::string& characterType, int health, int vitality, int attack, int defense);
	Character(const std::string& fileName);
	void exportCharacter(const std::string& fileName);
	void setName(const std::string& name);
	void setHealth(int health);
	void setVitality(int vitality);
	void setAttack(int attack);
	void setDefense(int defense);
	std::string getCharacterType() const;
	std::string getName() const;
	int getHealth() const;
	int getVitality() const;
	int getAttack() const;
	int getDefense() const;
private:
	int health;
	int vitality;
	int attack;
	int defense;
	std::string name;
	std::string characterType;
	void loadFromFile(const std::string& fileName);
};
