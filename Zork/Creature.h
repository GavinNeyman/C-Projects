#ifndef CREATURE_H
#define CREATURE_H
#include "Entity.h"
#include "Item.h"

class Creature : public Entity {
public:
	Creature();
	~Creature();
	//============================================================================================================================================================
	// Functions
	bool AttackWithWeapon(std::string, Item*);
	void AddVulnerability(std::string);
	void IncludeAttack(Trigger*);
	//============================================================================================================================================================
	// Variables
	Trigger* attack;
	std::vector<std::string> vulnerability;
};
#endif
