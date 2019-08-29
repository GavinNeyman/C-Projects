#ifndef INVENTORY_H
#define INVENTORY_H
#include "Entity.h"
#include "Holder.h"
#include "Item.h"
class Inventory : public Holder {
public:
	Inventory();
	~Inventory();
	//============================================================================================================================================================
	// Functions
	Item* Drop(std::string);
	void Read(std::string);
	void TurnOn(std::string);
	void CurrentItems();
};
#endif
