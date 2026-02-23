#ifndef __EX4_H__
#define __EX4_H__

#include <string>

const int MAX_SPELLS = 5;

enum ElementType {
    Fire,
    Ice,
    Lightning,
    Earth,
    Wind
};

const std::string elementTypeNames[] = {
    "Fire",
    "Ice",
    "Lightning",
    "Earth",
    "Wind"
};

class Exception {
    std::string message; // Error message.
public:
    // Constructor with error message.
    Exception(const std::string& msg);
    // Return the error message.
    std::string what() const;
};

struct Spell {
    std::string name;    // Name of the spell with at least 1 character.
    ElementType element; // Element type.
    int manaCost;        // Mana cost when casting the spell. Non-negative.

    Spell() : name(""), element(ElementType::Fire), manaCost(0) {}
    Spell(const std::string& n, ElementType e, int c) : name(n), element(e), manaCost(c) {}
};

class Spellbook {
protected:
    Spell spells[MAX_SPELLS];   // Stored spells.
    int spellCount;             // Number of learned spells.
    int maxMana;                // Maximum mana capacity.
    int currentMana;            // Current available mana.

public:
    /**
     * @brief Constructs a new spellbook.
     * 
     * Initializes: maxMana = 100, currentMana = 50, spellCount = 0.
     */
    Spellbook();

    /**
     * @brief Learns a new spell and adds it to the spellbook. Adds from index 0 to 4.
     * 
     * If there exists a spell in the spellbook whose name is the same with the new `spell`, overwrite it with the new one.
     * It has the priority over the Exception.
     * @param spell The spell to learn.
     * @throw Exception if the spellbook is full ("The spellbook is full!").
     */
    virtual void learnSpell(const Spell& spell);

    /**
     * @brief Casts a spell by name. (Not delete it from the spellbook.)
     * 
     * Print success message "Casted [name].\n" to stdout, and deducts mana.
     * @param spellName Name of the spell to cast.
     * @throw Exception if spell not found ("Spell [name] not learned!").
     * @throw Exception if not enough mana ("Not enough mana to cast [name]!").
     */
    void castSpell(const std::string& spellName);

    /**
     * @brief Prints all spells in index order (from 0 to 4) to stdout. Ignore empty slots.
     * 
     * Format for each spell: "[Name] ([Element]) - [Cost] mana.\n".
     * 
     * Ends with: "Total spells: [spellCount].\n".
     * @throw Exception if no spells ("Spellbook is empty!"). No other printing needed.
     */
    void printSpells() const;

    /**
     * @brief Restores mana (up to maxMana).
     * @param amount Mana to restore.
     * @throw Exception if amount <= 0 ("Restore amount must be positive!").
     * You must check it as long as the function is called.
     */
    void restoreMana(int amount);

    /**
     * @return Number of learned spells.
     */
    int getSpellCount() const { return spellCount; }

    /**
     * @return Current mana.
     */
    int getCurrentMana() const { return currentMana; }

    /**
     * @return Maximum mana limit.
     */
    int getMaxMana() const { return maxMana; }

    virtual ~Spellbook() = default;
};

class MasterSpellbook : public Spellbook {
private:
    ElementType forbiddenElement; // Element banned from casting
    int maxSpellCount;            // Maximum number of spells that can be learned

public:
    /**
     * @brief Constructs a MasterSpellbook.
     * 
     * Sets maxMana = 150, currentMana = 100, spellCount = 0.
     * 
     * Sets the forbidden element and the maximum number of spells that can be learned.
     * @param forbidden The element that cannot be learned.
     * @param maxSpells Maximum number of spells that can be learned. If maxSpells > MAX_SPELLS, set to MAX_SPELLS.
     * @throw Exception if maxSpells < 1 ("The master spellbook can hold at least 1 spell!").
     */
    MasterSpellbook(ElementType forbidden, int maxSpells);

    /**
     * @brief Overrides learnSpell: forbids learning spells of the banned element.
     * 
     * If there exists a spell in the spellbook whose name is the same with the new spell,
     * overwrite it with the new one. It has the priority over the following second Exception.
     * @throw Exception with message "[Element] is forbidden in the master spellbook!" 
     *        if trying to learn a banned spell. This exception has the priority over the second one.
     * @throw Exception if the spellbook is full ("The master spellbook is full!").
     */
    void learnSpell(const Spell& spell) override;

    /**
     * @return The forbidden element.
     */
    ElementType getForbiddenElement() const { return forbiddenElement; }

    /**
     * @return Maximum number of spells that can be learned.
     */
    int getMaxSpellCount() const { return maxSpellCount; }

    ~MasterSpellbook() = default;
};

#endif // __EX4_H__
