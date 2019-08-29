#include "Creature.h"

Creature::Creature(){
}


Creature::~Creature(){
}

bool Creature::AttackWithWeapon(std::string c, Item* i) {
	for (std::size_t j = 0; j < vulnerability.size(); ++j) {
		if (vulnerability[j] == i->name) {
			std::cout << "You Assult the " << c << " with the " << i->name << std::endl;
			return true;
		}
	}
	return false;
}

void Creature::AddVulnerability(std::string v) {
	vulnerability.push_back(v);
}

void Creature::IncludeAttack(Trigger* t) {
	attack = t;
}
