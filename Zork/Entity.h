#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>
#include <string>
#include <vector>
#include "Trigger.h"
class Entity{
public:
	Entity();
	~Entity();
	void IncludeTrigger(Trigger* t);
	void SetName(std::string);
	void SetDescription(std::string);
	void SetStatus(std::string);
	std::string name;
	std::string status;
	std::string description;
	std::vector<Trigger*> trigger;
};

#endif