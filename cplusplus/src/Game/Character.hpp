#pragma once
#include <cmath>
#include <string>

class Character {
public:
    Character(const std::string& name, const std::string& characterType, int vitality, int attack, int defense, int stamina);
    Character(const std::string& fileName);
    Character() = default;
    void exportCharacter(const std::string& fileName) const;
    void loadFromFile(const std::string& fileName);
    void setName(const std::string& name);
    void setHealth(int health);
    void setVitality(int vitality);
    void setAttack(int attack);
    void setDefense(int defense);
    void setStamina(int stamina);
    std::string getCharacterType() const;
    std::string getName() const;
    int getHealth() const;
    int getVitality() const;
    int getAttack() const;
    int getDefense() const;
    int getStamina() const;

    void takeDamage(int damage);
    void heal();
public:
    static const int healthMultiplier = 5;
private:
    int health;
    int vitality;
    int attack;
    int defense;
    int stamina;
    std::string name;
    std::string characterType;


    /// <summary>
    /// Exponential Growth formula used by leveling system.
    /// </summary>
    static float expGrowth(const float initial, const float growth, const float time) {
        const float value = initial * std::pow(1.0f + growth, time);
        return value;
    }
};
