#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class TextRenderer;
class SpriteRenderer;

class CreateCharacterController final : public ScriptableBehaviour {
public:
    CreateCharacterController(SpriteRenderer* characterImage, TextRenderer* characterNameText, TextRenderer* characterTypeText, TextRenderer* vitalityText,
                              TextRenderer* attackText, TextRenderer* defenseText, TextRenderer* staminaText, TextRenderer* remainingPointsText);

    CreateCharacterController(const std::string& identifier, SpriteRenderer* characterImage, TextRenderer* characterNameText, TextRenderer* characterTypeText,
                              TextRenderer* vitalityText, TextRenderer* damageText, TextRenderer* defenseText, TextRenderer* staminaText, TextRenderer* remainingPointsText);

    void changeVitality(int amount);
    void changeAttack(int amount);
    void changeDefense(int amount);
    void changeStamina(int amount);
    void randomizeName();
    void randomizeStats();
    void changeTypeIndex(int amount);
    void exportCharacter() const;
    std::string getExportPath() const;
protected:
    void onStart() override;

private:
    void calculateRemainingPoints();
    void changeTypeImage();
    void loadBaseStats();

    int remainingPoints;
    int maxPoints;

    int vitalityPoints;
    int attackPoints;
    int defensePoints;
    int staminaPoints;
    int namesIndex;
    int typesIndex;

    SpriteRenderer* characterImage;
    TextRenderer* characterNameText;
    TextRenderer* characterTypeText;
    TextRenderer* remainingPointsText;
    TextRenderer* vitalityText;
    TextRenderer* attackText;
    TextRenderer* defenseText;
    TextRenderer* staminaText;
    const inline static int maxTypes = 5;
    const inline static std::string types[maxTypes] = {"player", "adventurer", "female", "soldier", "zombie"};
    const inline static std::string names[60] = {
        "Sammie", "Regina", "Freddie", "Enrique", "Mignon", "Vanna", "Jaime", "Len", "Deloris", "Jodee", "Robby", "Mckenzie", "Rodrigo", "Emmett", "Cathryn", "Edmundo", "Darell",
        "Tyrell", "Hildegarde", "Julianne", "Marylou", "Andy", "Vilma", "Gala", "Linwood", "Riley", "Charlena", "Crissy", "Jeremy", "Ruby", "Williemae", "Ashlyn", "Elizabet",
        "Donte", "Gerry", "Rico", "Marinda", "Alfonso", "Shavon", "Solange", "Mayola", "Randy", "Richard", "Leonel", "Rufina", "Earnest", "Cortez", "Teodoro", "Rhett", "Ruthe",
        "Vicky", "Alice", "Yong", "Toya", "Machelle", "Jayne", "Zachariah", "Josie", "Steven", "Wilfredo"
    };

    struct base_stats {
        int vitality;
        int attack;
        int defense;
        int stamina;
    };
    inline static base_stats baseStats[maxTypes] = {{},{},{},{},{}};
};
