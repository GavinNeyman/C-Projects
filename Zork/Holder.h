#ifndef HOLDER_H
#define HOLDER_H
#include "Entity.h"
#include "Item.h"
class Holder : public Entity {
public:
	Holder();
	~Holder();
	//============================================================================================================================================================
	// Functions
	virtual void Add(Item*); // Add an item to the holder
	virtual void Delete(Item*); // Remove an item from the holder
	virtual void Put(Item*); // Put an item into the holder and display an appropriate message
	virtual bool Have(std::string); // Check to see if the holder contains a certain item
	virtual Item* GetItem(std::string); // Remove an item from the holder and return it
	void PushItem(std::string i);
	void AddItems(std::vector<Item*>);
	//============================================================================================================================================================
	// Variables
	std::vector<std::string> itemList;
	std::vector<Item*> item;
};
#endif
