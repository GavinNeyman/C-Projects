#ifndef ROOM_H
#define ROOM_H
#include "Entity.h"
#include "Holder.h"
#include "Container.h"
#include "Creature.h"
#include "Trigger.h"
#include "Boarder.h"
#include "Inventory.h"
class Room : public Holder{
public:
	Room();
	~Room();
	//============================================================================================================================================================
	// Functions
	void PlayIntro();
	void PutInContainer(std::string, Item*);
	bool HaveContainer(std::string);
	bool HaveCreature(std::string);
	bool HaveBoarder(std::string);
	void OpenContainer(std::string);
	std::string Move(std::string);
	void PushCreature(std::string);
	void PushContainer(std::string);
	void IncludeBoarder(Boarder*);
	void CheckTriggers(Inventory*, std::vector<Item*>, std::vector<Container*>, std::vector<Creature*>, std::vector<Room*>);
	int ActivateTrigger(std::string, Inventory*, std::vector<Item*>, std::vector<Container*>, std::vector<Creature*>, std::vector<Room*>);
	void AddCreatures(std::vector<Creature*>);
	void AddContainers(std::vector<Container*>);
	void BehindTheScenes(std::string, std::vector<Item*>, std::vector<Container*>, std::vector<Creature*>, std::vector<Room*>, Inventory*);
	void SetType(std::string);
	void ClearContainer(std::string);
	void TurnOnItem(std::string, std::vector<Item*>, std::vector<Container*>, std::vector<Creature*>, std::vector<Room*>, Inventory* inv);
	void AttackCreature(std::string, std::string, Inventory*, std::vector<Item*>, std::vector<Container*>, std::vector<Creature*>, std::vector<Room*>);
	bool AttackWithWeapon(std::string, Item*, Creature*, Inventory*, std::vector<Item*>, std::vector<Container*>, std::vector<Creature*>, std::vector<Room*>);
	//============================================================================================================================================================
	// Variables
	std::string type;
	std::vector<Boarder*> boarder;
	std::vector<Container*> container;
	std::vector<Creature*> creature;
	std::vector<std::string> containerList;
	std::vector<std::string> creatureList;
};
#endif

