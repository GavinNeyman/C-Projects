#include "Entity.h"

Entity::Entity(){
	status = "";
}

Entity::~Entity(){
}

void Entity::IncludeTrigger(Trigger* t) {
	trigger.push_back(t);
}

void Entity::SetName(std::string arg) {
	name = arg;
}

void Entity::SetDescription(std::string arg) {
	description = arg;
}

void Entity::SetStatus(std::string arg) {
	status = arg;
}