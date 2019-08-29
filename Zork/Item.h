#ifndef ITEM_H
#define ITEM_H
#include "Entity.h"

class Item : public Entity {
public:
	Item();
	~Item();
	//============================================================================================================================================================
	// Functions
	std::string GetName();
	void TurnOn();
	void AddAction(std::string);
	void SetWriting(std::string);
	void SetPrint(std::string);
	//============================================================================================================================================================
	// Variables
	std::string writing;
	std::string print;
	std::vector<std::string> action;
	
};
#endif

