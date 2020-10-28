#pragma once
#include <cmath>
#include <string>
#include <fstream>

class Character {
public:
    Character(const std::string& name, const std::string& characterType, int vitality, int attack, int defense, int stamina);
    Character(const std::string& fileName);
    Character();
    
// Getters and setters
    void setName(const std::string& name);
    void setCharacterType(const std::string& characterType);
    void setHealth(int health);
    void setVitality(int vitality);
    void setAttack(int attack);
    void setDefense(int defense);
    void setStamina(int stamina);
    std::string getName() const;
    std::string getCharacterType() const;
    int getHealth() const;
    int getVitality() const;
    int getAttack() const;
    int getDefense() const;
    int getStamina() const;

// Interaction functions
    int getMaxHealth() const;
    int getAttackPower() const;
    int getDefensePower() const;
    int getHealPower() const;

    bool takeDamage(int damage);
    void heal(int healPower);

    void setIsPlayer(bool isPlayer);
    void randomize(int maxPoints);
    void loadFromCharacter(Character& character);
    void loadFromFile(const std::string& fileName);
    void exportCharacter(const std::string& fileName, int openMode = std::ios::out) const;
public://static
    static void loadBaseStats();
    static const int healthMultiplier = 5;
private:
    int health;
    int vitality;
    int attack;
    int defense;
    int stamina;
    std::string name;
    std::string characterType;
    bool isPlayer;

    bool canEvade() const;
private://static
    const inline static int maxTypes = 5;
    const inline static std::string types[maxTypes] = { "player", "adventurer", "female", "soldier", "zombie" };
    
    struct base_stats {
        int vitality;
        int attack;
        int defense;
        int stamina;
    };
    inline static base_stats baseStats[maxTypes] = { {},{},{},{},{} };
    inline static bool loadedBaseStats = false;
    
    /// <summary>
    /// Exponential Growth formula used by leveling system.
    /// </summary>
    static float expGrowth(const float initial, const float growth, const float time) {
        const float value = initial * std::pow(1.0f + growth, time);
        return value;
    }
};
