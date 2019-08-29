#include "Inventory.h"

Inventory::Inventory() {
	name = "inventory";
}


Inventory::~Inventory() {
}

Item* Inventory::Drop(std::string i) {
	for (std::size_t j = 0; j < item.size(); ++j) {
		if (item[j]->name == i) {
			Item* found = item[j];
			std::cout << i << " dropped." << std::endl;
			Delete(item[j]);
			found->description = i + " is laying on the ground.";
			return found;
		}
	}
}

void Inventory::Read(std::string i) {
	for (std::size_t j = 0; j < item.size(); ++j) {
		if (item[j]->name == i) {
			std::cout << item[j]->writing << std::endl;
			return;
		}
	}
}

void Inventory::TurnOn(std::string i) {
	for (std::size_t j = 0; j < item.size(); ++j) {
		if (item[j]->name == i) {
			if (item[j]->status == "off" || item[j]->status == "on") {
				item[j]->TurnOn();
				std::cout << "You activate the " << i << std::endl;
			}
			else {
				std::cout << i << " can not be turned on." << std::endl;
			}
		}
	}
}

void Inventory::CurrentItems() {
	if (item.size() == 0) {
		std::cout << "Inventory: empty" << std::endl;
	}
	else {
		std::cout << "Inventory:";
		for (std::size_t i = 0; i < item.size(); ++i) {
			std::cout << " ";
			std::cout << item[i]->name;
		}
		std::cout << std::endl;
	}
}