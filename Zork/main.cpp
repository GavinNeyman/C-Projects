#include <sstream>
#include "Entity.h"
#include "Room.h"
#include "Inventory.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"
using namespace std;
using namespace rapidxml;

bool GAME = true;
Room* currRoom = NULL;
int main() {
	std::cout << "Please enter the name of the XML document you would like to load." << std::endl;
	string xmlName;
	std::getline(std::cin, xmlName);
	const char* fileName = xmlName.c_str();

	rapidxml::file<> xmlFile(fileName);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* root = doc.first_node(); // map

	vector<Container*> container;
	vector<Room*> room;
	vector<Item*> item;
	vector<Creature*> creature;
	string check;

	for (xml_node<>* xmlnode = root->first_node(); xmlnode; xmlnode = xmlnode->next_sibling()) {
		check = xmlnode->name();
		if (check == "room") {
			Room* tmpRoom = new Room;
			for (xml_node<>* xmlnode2 = xmlnode->first_node(); xmlnode2; xmlnode2 = xmlnode2->next_sibling()) {
				check = xmlnode2->name();
				if (check == "name") {
					tmpRoom->SetName(xmlnode2->value());
				}
				else if (check == "description") {
					tmpRoom->SetDescription(xmlnode2->value());
				}
				else if (check == "item") {
					tmpRoom->PushItem(xmlnode2->value());
				}
				else if (check == "container") {
					tmpRoom->PushContainer(xmlnode2->value());
				}
				else if (check == "creature") {
					tmpRoom->PushCreature(xmlnode2->value());
				}
				else if (check == "border") {
					Boarder* tmpBoarder = new Boarder;
					for (xml_node<>* xmlnode3 = xmlnode2->first_node(); xmlnode3; xmlnode3 = xmlnode3->next_sibling()) {
						check = xmlnode3->name();
						if (check == "direction") {
							tmpBoarder->SetDirection(xmlnode3->value());
						}
						else if (check == "name") {
							tmpBoarder->SetName(xmlnode3->value());
						}
					}
					tmpRoom->IncludeBoarder(tmpBoarder);
				}
				else if (check == "trigger") {
					Trigger* tmpTrigger = new Trigger;
					for (xml_node<>* xmlnode3 = xmlnode2->first_node(); xmlnode3; xmlnode3 = xmlnode3->next_sibling()) {
						check = xmlnode3->name();
						if (check == "command") {
							tmpTrigger->SetCommand(xmlnode3->value());
						}
						else if (check == "type") {
							tmpTrigger->SetType(xmlnode3->value());
						}
						else if (check == "condition") {
							for (xml_node<>* xmlnode4 = xmlnode3->first_node(); xmlnode4; xmlnode4 = xmlnode4->next_sibling()) {
								check = xmlnode4->name();
								if (check == "has") {
									tmpTrigger->SetHas(xmlnode4->value());
								}
								else if (check == "owner") {
									tmpTrigger->SetOwner(xmlnode4->value());
								}
								else if (check == "object") {
									tmpTrigger->SetObject(xmlnode4->value());
								}
								else if (check == "status") {
									tmpTrigger->SetStatus(xmlnode4->value());
								}
							}
						}
						else if (check == "print") {
							tmpTrigger->AddPrint(xmlnode3->value());
						}
						else if (check == "action") {
							tmpTrigger->AddAction(xmlnode3->value());
						}
					}
					tmpRoom->IncludeTrigger(tmpTrigger);
				}
				else if (check == "type") {
					tmpRoom->SetType(xmlnode2->value());
				}
				else if (check == "status") {
					tmpRoom->SetStatus(xmlnode2->value());
				}
			}
			room.push_back(tmpRoom);
		}
		else if (check == "item") {
			Item* tmpItem = new Item;
			for (xml_node<>* xmlnode2 = xmlnode->first_node(); xmlnode2; xmlnode2 = xmlnode2->next_sibling()) {
				check = xmlnode2->name();
				if (check == "name") {
					tmpItem->SetName(xmlnode2->value());
				}
				else if (check == "description") {
					tmpItem->SetDescription(xmlnode2->value());
				}
				else if (check == "status") {
					tmpItem->SetStatus(xmlnode2->value());
				}
				else if (check == "trigger") {
					Trigger* tmpTrigger = new Trigger;
					for (xml_node<>* xmlnode3 = xmlnode2->first_node(); xmlnode3; xmlnode3 = xmlnode3->next_sibling()) {
						check = xmlnode3->name();
						if (check == "command") {
							tmpTrigger->SetCommand(xmlnode3->value());
						}
						else if (check == "type") {
							tmpTrigger->SetType(xmlnode3->value());
						}
						else if (check == "condition") {
							for (xml_node<>* xmlnode4 = xmlnode3->first_node(); xmlnode4; xmlnode4 = xmlnode4->next_sibling()) {
								xmlnode4->name();
								if (check == "has") {
									tmpTrigger->SetHas(xmlnode4->value());
								}
								else if (check == "owner") {
									tmpTrigger->SetOwner(xmlnode4->value());
								}
								else if (check == "object") {
									tmpTrigger->SetObject(xmlnode4->value());
								}
								else if (check == "status") {
									tmpTrigger->SetStatus(xmlnode4->value());
								}
							}
						}
						else if (check == "print") {
							tmpTrigger->AddPrint(xmlnode3->value());
						}
						else if (check == "action") {
							tmpTrigger->AddAction(xmlnode3->value());
						}
					}
					tmpItem->IncludeTrigger(tmpTrigger);
				}
				else if (check == "writing") {
					tmpItem->SetWriting(xmlnode2->value());
				}
				else if (check == "turnon") {
					for (xml_node<>* xmlnode3 = xmlnode2->first_node(); xmlnode3; xmlnode3 = xmlnode3->next_sibling()) {
						check = xmlnode3->name();
						if (check == "print") {
							tmpItem->SetPrint(xmlnode3->value());
						}
						else if (check == "action") {
							tmpItem->AddAction(xmlnode3->value());
						}
					}
				}
			}
			item.push_back(tmpItem);
		}
		else if (check == "container") {
			Container* tmpContainer = new Container;
			for (xml_node<>* xmlnode2 = xmlnode->first_node(); xmlnode2; xmlnode2 = xmlnode2->next_sibling()) {
				check = xmlnode2->name();
				if (check == "name") {
					tmpContainer->SetName(xmlnode2->value());
				}
				else if (check == "description") {
					tmpContainer->SetDescription(xmlnode2->value());
				}
				else if (check == "item") {
					tmpContainer->PushItem(xmlnode2->value());
				}
				else if (check == "trigger") {
					Trigger* tmpTrigger = new Trigger;
					for (xml_node<>* xmlnode3 = xmlnode2->first_node(); xmlnode3; xmlnode3 = xmlnode3->next_sibling()) {
						check = xmlnode3->name();
						if (check == "command") {
							tmpTrigger->SetCommand(xmlnode3->value());
						}
						else if (check == "type") {
							tmpTrigger->SetType(xmlnode3->value());
						}
						else if (check == "condition") {
							for (xml_node<>* xmlnode4 = xmlnode3->first_node(); xmlnode4; xmlnode4 = xmlnode4->next_sibling()) {
								check = xmlnode4->name();
								if (check == "has") {
									tmpTrigger->SetHas(xmlnode4->value());
								}
								else if (check == "owner") {
									tmpTrigger->SetOwner(xmlnode4->value());
								}
								else if (check == "object") {
									tmpTrigger->SetObject(xmlnode4->value());
								}
								else if (check == "status") {
									tmpTrigger->SetStatus(xmlnode4->value());
								}
							}
						}
						else if (check == "print") {
							tmpTrigger->AddPrint(xmlnode3->value());
						}
						else if (check == "action") {
							tmpTrigger->AddAction(xmlnode3->value());
						}
					}
					tmpContainer->IncludeTrigger(tmpTrigger);
				}
				else if (check == "status") {
					tmpContainer->SetStatus(xmlnode2->value());
				}
				else if (check == "accept") {
					tmpContainer->AddAccept(xmlnode2->value());
				}
			}
			container.push_back(tmpContainer);
		}
		else if (check == "creature") {
			Creature* tmpCreature = new Creature;
			for (xml_node<>* xmlnode2 = xmlnode->first_node(); xmlnode2; xmlnode2 = xmlnode2->next_sibling()) {
				check = xmlnode2->name();
				if (check == "name") {
					tmpCreature->SetName(xmlnode2->value());
				}
				else if (check == "description") {
					tmpCreature->SetDescription(xmlnode2->value());
				}
				else if (check == "status") {
					tmpCreature->SetStatus(xmlnode2->value());
				}
				else if (check == "vulnerability") {
					tmpCreature->AddVulnerability(xmlnode2->value());
				}
				else if (check == "trigger") {
					Trigger* tmpTrigger = new Trigger;
					for (xml_node<>* xmlnode3 = xmlnode2->first_node(); xmlnode3; xmlnode3 = xmlnode3->next_sibling()) {
						check = xmlnode3->name();
						if (check == "command") {
							tmpTrigger->SetCommand(xmlnode3->value());
						}
						else if (check == "type") {
							tmpTrigger->SetType(xmlnode3->value());
						}
						else if (check == "condition") {
							for (xml_node<>* xmlnode4 = xmlnode3->first_node(); xmlnode4; xmlnode4 = xmlnode4->next_sibling()) {
								check = xmlnode4->name();
								if (check == "has") {
									tmpTrigger->SetHas(xmlnode4->value());
								}
								else if (check == "owner") {
									tmpTrigger->SetOwner(xmlnode4->value());
								}
								else if (check == "object") {
									tmpTrigger->SetObject(xmlnode4->value());
								}
								else if (check == "status") {
									tmpTrigger->SetStatus(xmlnode4->value());
								}
							}
						}
						else if (check == "print") {
							tmpTrigger->AddPrint(xmlnode3->value());
						}
						else if (check == "action") {
							tmpTrigger->AddAction(xmlnode3->value());
						}
					}
					tmpCreature->IncludeTrigger(tmpTrigger);
				}
				else if (check == "attack") {
					Trigger* tmpAttack = new Trigger;
					for (xml_node<>* xmlnode3 = xmlnode2->first_node(); xmlnode3; xmlnode3 = xmlnode3->next_sibling()) {
						check = xmlnode3->name();
						if (check == "command") {
							tmpAttack->SetCommand(xmlnode3->value());
						}
						else if (check == "type") {
							tmpAttack->SetType(xmlnode3->value());
						}
						else if (check == "condition") {
							for (xml_node<>* xmlnode4 = xmlnode3->first_node(); xmlnode4; xmlnode4 = xmlnode4->next_sibling()) {
								check = xmlnode4->name();
								if (check == "has") {
									tmpAttack->SetHas(xmlnode4->value());
								}
								else if (check == "owner") {
									tmpAttack->SetOwner(xmlnode4->value());
								}
								else if (check == "object") {
									tmpAttack->SetObject(xmlnode4->value());
								}
								else if (check == "status") {
									tmpAttack->SetStatus(xmlnode4->value());
								}
							}
						}
						else if (check == "print") {
							tmpAttack->AddPrint(xmlnode3->value());
						}
						else if (check == "action") {
							tmpAttack->AddAction(xmlnode3->value());
						}
					}
					tmpCreature->IncludeAttack(tmpAttack);
				}
			}
			creature.push_back(tmpCreature);
		}
	}

	// ==========================================================================================================================================================
	// Initialize Variables
	for (std::size_t j = 0; j < room.size(); ++j) {
		room[j]->AddItems(item);
		room[j]->AddCreatures(creature);
		room[j]->AddContainers(container);
		for (std::size_t k = 0; k < room[j]->container.size(); ++k) {
			room[j]->container[k]->AddItems(item);
		}
		if (room[j]->name == "Entrance") {
			currRoom = room[j];
		}
	}
	Inventory* inv = new Inventory();
	// ==========================================================================================================================================================
	// Take Commands
	std::string input;
	std::string itemCommand;
	std::string containerCommand;
	std::string creatureCommand;
	std::string throwAway;
	std::string userCommand;
	std::string moveRoom;
	int triggerTest;
	currRoom->PlayIntro();
	while (GAME) {
		currRoom->CheckTriggers(inv, item, container, creature, room);
		std::getline(std::cin, input);
		stringstream command(input);
		command >> userCommand;
		triggerTest = 0;
		if (userCommand == "n") {
			triggerTest = currRoom->ActivateTrigger("n", inv, item, container, creature, room);
			if (currRoom->HaveBoarder("north") && (triggerTest == 0 || triggerTest == 2)) {
				moveRoom = currRoom->Move("north");
				for (std::size_t j = 0; j < room.size(); ++j) {
					if (room[j]->name == moveRoom) {
						currRoom = room[j];
						break;
					}
				}
				currRoom->PlayIntro();
			}
		}
		else if (userCommand == "s") {
			triggerTest = currRoom->ActivateTrigger("s", inv, item, container, creature, room);
			if (currRoom->HaveBoarder("south") && (triggerTest == 0 || triggerTest == 2)) {
				moveRoom = currRoom->Move("south");
				for (std::size_t j = 0; j < room.size(); ++j) {
					if (room[j]->name == moveRoom) {
						currRoom = room[j];
						break;
					}
				}
				currRoom->PlayIntro();
			}
		}
		else if (userCommand == "e") {
			triggerTest = currRoom->ActivateTrigger("e", inv, item, container, creature, room);
			if (currRoom->HaveBoarder("east") && (triggerTest == 0 || triggerTest == 2)) {
				moveRoom = currRoom->Move("east");
				for (std::size_t j = 0; j < room.size(); ++j) {
					if (room[j]->name == moveRoom) {
						currRoom = room[j];
						break;
					}
				}
				currRoom->PlayIntro();
			}
		}
		else if (userCommand == "w") {
			triggerTest = currRoom->ActivateTrigger("w", inv, item, container, creature, room);
			if (currRoom->HaveBoarder("west") && (triggerTest == 0 || triggerTest == 2)) {
				moveRoom = currRoom->Move("west");
				for (std::size_t j = 0; j < room.size(); ++j) {
					if (room[j]->name == moveRoom) {
						currRoom = room[j];
						break;
					}
				}
				currRoom->PlayIntro();
			}
		}
		else if (userCommand == "i") {
			triggerTest = currRoom->ActivateTrigger("i", inv, item, container, creature, room);
			if (triggerTest == 0 || triggerTest == 2) {
				inv->CurrentItems();
			}
		}
		else if (userCommand == "take") {
			command >> itemCommand;
			triggerTest = currRoom->ActivateTrigger("take " + itemCommand, inv, item, container, creature, room);
			if (triggerTest == 0 || triggerTest == 2) {
				if (currRoom->Have(itemCommand)) {
					Item* found = currRoom->GetItem(itemCommand);
					currRoom->ClearContainer(itemCommand);
					inv->Put(found);
				}
			}
		}
		else if (userCommand == "open") {
			command >> containerCommand;
			triggerTest = currRoom->ActivateTrigger("open " + containerCommand, inv, item, container, creature, room);
			if (triggerTest == 0 || triggerTest == 2) {
				if (containerCommand == "exit") {
					if (currRoom->type == "exit") {
						std::cout << "Game Over" << std::endl;
						GAME = false;
					}
					else {
						std::cout << "Error" << std::endl;
					}
				}
				else if (currRoom->HaveContainer(containerCommand)) {
					currRoom->OpenContainer(containerCommand);
				}
			}
		}
		else if (userCommand == "read") {
			command >> itemCommand;
			triggerTest = currRoom->ActivateTrigger("read " + itemCommand, inv, item, container, creature, room);
			if (triggerTest == 0 || triggerTest == 2) {
				if (inv->Have(itemCommand)) {
					inv->Read(itemCommand);
				}
			}
		}
		else if (userCommand == "drop") {
			command >> itemCommand;
			triggerTest = currRoom->ActivateTrigger("drop " + itemCommand, inv, item, container, creature, room);
			if (triggerTest == 0 || triggerTest == 2) {
				if (inv->Have(itemCommand)) {
					Item* found = inv->Drop(itemCommand);
					currRoom->Add(found);
				}
			}
		}
		else if (userCommand == "put") {
			command >> itemCommand;
			if (inv->Have(itemCommand)) {
				command >> throwAway; // get rid of "in"
				command >> containerCommand;
				triggerTest = currRoom->ActivateTrigger("put " + itemCommand, inv, item, container, creature, room);
				if (triggerTest == 0 || triggerTest == 2) {
					if (currRoom->HaveContainer(containerCommand)) {
						Item* i = inv->GetItem(itemCommand);
						currRoom->PutInContainer(containerCommand, i);
					}
				}
			}
		}
		else if (userCommand == "turn") {
			command >> throwAway;
			if (throwAway == "on") {
				command >> itemCommand;
				triggerTest = currRoom->ActivateTrigger("turn on " + itemCommand, inv, item, container, creature, room);
				if (triggerTest == 0 || triggerTest == 2) {
					if (inv->Have(itemCommand)) {
						currRoom->TurnOnItem(itemCommand, item, container, creature, room, inv);
					}
				}
			}
		}
		else if (userCommand == "attack") {
			command >> creatureCommand;
			if (currRoom->HaveCreature(creatureCommand)) {
				command >> throwAway; // get rid of "with"
				command >> itemCommand;
				triggerTest = currRoom->ActivateTrigger("attack " + creatureCommand + " with " + itemCommand, inv, item, container, creature, room);
				if (triggerTest == 0) {
					if (inv->Have(itemCommand)) {
						currRoom->AttackCreature(creatureCommand, itemCommand, inv, item, container, creature, room);
					}
				}
				else if (triggerTest == 2) {
					if (inv->Have(itemCommand)) {
						std::cout << "You Assult the " << creatureCommand << " with the " << itemCommand << std::endl;
					}
				}
			}
		}
		else {
			std::cout << "Error" << std::endl;
		}
	}
	system("pause");
}