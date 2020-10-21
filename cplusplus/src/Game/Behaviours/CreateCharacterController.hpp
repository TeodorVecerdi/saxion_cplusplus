#pragma once
#include "Core/ScriptableBehaviour.hpp"

class TextRenderer;
class SpriteRenderer;

class CreateCharacterController final : public ScriptableBehaviour {
public:
    CreateCharacterController(int maxPoints, SpriteRenderer* characterImage, TextRenderer* characterNameText, TextRenderer* characterTypeText, TextRenderer* vitalityText,
                              TextRenderer* damageText, TextRenderer* defenseText, TextRenderer* remainingPointsText);

    CreateCharacterController(const std::string& identifier, int maxPoints, SpriteRenderer* characterImage, TextRenderer* characterNameText, TextRenderer* characterTypeText,
                              TextRenderer* vitalityText, TextRenderer* damageText, TextRenderer* defenseText, TextRenderer* remainingPointsText);

    void changeVitality(int amount);
    void changeDamage(int amount);
    void changeDefense(int amount);
    void randomizeName();
    void changeTypeIndex(int amount);
protected:
    void onStart() override;

private:
    void calculateRemainingPoints();
    void changeTypeImage();

    int remainingPoints;
    int maxPoints;
    int vitalityPoints;
    int damagePoints;
    int defensePoints;
    int namesIndex;
    int typesIndex;

    SpriteRenderer* characterImage;
    TextRenderer* characterNameText;
    TextRenderer* characterTypeText;
    TextRenderer* vitalityText;
    TextRenderer* damageText;
    TextRenderer* defenseText;
    TextRenderer* remainingPointsText;
    const std::string types[5] = {"player", "adventurer", "female", "soldier", "zombie"};
    const std::string names[60] = {
        "Sammie", "Regina", "Freddie", "Enrique", "Mignon", "Vanna", "Jaime", "Len", "Deloris", "Jodee", "Robby", "Mckenzie", "Rodrigo", "Emmett", "Cathryn", "Edmundo", "Darell",
        "Tyrell", "Hildegarde", "Julianne", "Marylou", "Andy", "Vilma", "Gala", "Linwood", "Riley", "Charlena", "Crissy", "Jeremy", "Ruby", "Williemae", "Ashlyn", "Elizabet",
        "Donte", "Gerry", "Rico", "Marinda", "Alfonso", "Shavon", "Solange", "Mayola", "Randy", "Richard", "Leonel", "Rufina", "Earnest", "Cortez", "Teodoro", "Rhett", "Ruthe",
        "Vicky", "Alice", "Yong", "Toya", "Machelle", "Jayne", "Zachariah", "Josie", "Steven", "Wilfredo"
    };
};
