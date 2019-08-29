#include "Room.h"
#include <sstream>

extern bool GAME;
extern Room* currRoom;

Room::Room(){
	type = "normal";
}

Room::~Room(){}

void Room::PlayIntro() {
	std::cout << description;
	for (std::size_t j = 0; j < container.size(); ++j) {
		std::cout << container[j]->description;
	}
	for (std::size_t j = 0; j < creature.size(); ++j) {
		std::cout << creature[j]->description;
	}
	for (std::size_t j = 0; j < item.size(); ++j) {
		std::cout << item[j]->description;
	}
	std::cout << std::endl;
}

void Room::PutInContainer(std::string c, Item* i) {
	for (std::size_t j = 0; j < container.size(); ++j) {
		if (container[j]->name == c) {
			if(container[j]->CheckAcceptance(i->name)) {
				i->description = "";
				container[j]->Put(i);
				container[j]->itemList.push_back(i->name);
			}
			else {
				std::cout << "Error" << std::endl;
			}
			return;
		}
	}
}

bool Room::HaveContainer(std::string i) {
	for (std::size_t j = 0; j < container.size(); ++j) {
		if (container[j]->name == i) {
			return true;
		}
	}
	std::cout << "Error" << std::endl;
	return false;
}

bool Room::HaveCreature(std::string c) {
	for (std::size_t j = 0; j < creature.size(); ++j) {
		if (creature[j]->name == c) {
			return true;
		}
	}
	std::cout << "Error" << std::endl;
	return false;
}

void Room::OpenContainer(std::string i) {
	for (std::size_t j = 0; j < container.size(); ++j) {
		if (container[j]->name == i) {
			std::vector<Item*> list = container[j]->ClearContents();
			for (std::size_t k = 0; k < list.size(); ++k) {
				item.push_back(list[k]);
			}
			container[j]->PrintContents();
			return;
		}
	}
}

void Room::ClearContainer(std::string i) {
	for (std::size_t j = 0; j < container.size(); ++j) {
		for (std::size_t k = 0; k < container[j]->itemList.size(); ++k) {
			if (container[j]->itemList[k] == i) {
				container[j]->itemList.erase(container[j]->itemList.begin() + k);
				return;
			}
		}
	}
}

bool Room::HaveBoarder(std::string dir) {
	for (std::size_t j = 0; j < boarder.size(); ++j) {
		if (boarder[j]->direction == dir) {
			return true;
		}
	}
	std::cout << "Can't go that way." << std::endl;
	return false;
}

std::string Room::Move(std::string dir) {
	for (std::size_t j = 0; j < boarder.size(); ++j) {
		if (boarder[j]->direction == dir) {
			return boarder[j]->name;
		}
	}
	return "Error";
}

void Room::PushCreature(std::string c) {
	creatureList.push_back(c);
}

void Room::PushContainer(std::string c) {
	containerList.push_back(c);
}

void Room::IncludeBoarder(Boarder* b) {
	boarder.push_back(b);
}

void Room::CheckTriggers(Inventory* inv, std::vector<Item*> i, std::vector<Container*> c, std::vector<Creature*> creeps, std::vector<Room*> r) {
	for (std::size_t j = 0; j < trigger.size(); ++j) {
		if (trigger[j]->command == "") {
			bool has = false;
			//================================================================================================================================================
			// Check OWNER-HAS-OBJECT trigger
			// Check to see if inv is owner and has object
			if (trigger[j]->condition.owner.owner == "inventory") {
				for (std::size_t k = 0; k < inv->item.size(); ++k) {
					if (inv->item[k]->name == trigger[j]->condition.owner.object) {
						has = true;
						break;
					}
				}
			}
			// Check to see if room is owner and has object
			else if (trigger[j]->condition.owner.owner == name) {
				for (std::size_t k = 0; k < item.size(); ++k) {
					if (item[k]->name == trigger[j]->condition.owner.object) {
						has = true;
						break;
					}
				}
			}
			// Check containers in room for owner and check if it has object
			else {
				for (std::size_t k = 0; k < container.size(); ++k) {
					if (container[k]->name == trigger[j]->condition.owner.owner) {
						for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
							if (container[k]->item[n]->name == trigger[j]->condition.owner.object) {
								has = true;
								break;
							}
						}
						break;
					}
				}
			}
			if ((trigger[j]->condition.owner.has == "yes" && has) || (trigger[j]->condition.owner.has == "no" && !has)) {
				std::cout << trigger[j]->print << std::endl;
				for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
					BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
				}
				if (trigger[j]->type == "single") {
					trigger.erase(trigger.begin() + j);
				}
				return;
			}

			//================================================================================================================================================
			// Check OBJECT-STATUS trigger
			// Check for object in inventory
			std::string status;
			for (std::size_t k = 0; k < inv->item.size(); ++k) {
				if (inv->item[k]->name == trigger[j]->condition.status.object) {
					if (inv->item[k]->status == trigger[j]->condition.status.status) {
						std::cout << trigger[j]->print << std::endl;
						for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
							BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
						}
						if (trigger[j]->type == "single") {
							trigger.erase(trigger.begin() + j);
						}
						return;
					}
				}
			}
			// Check for object in room
			for (std::size_t k = 0; k < item.size(); ++k) {
				if (item[k]->name == trigger[j]->condition.status.object) {
					if (item[k]->status == trigger[j]->condition.status.status) {
						std::cout << trigger[j]->print << std::endl;
						for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
							BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
						}
						if (trigger[j]->type == "single") {
							trigger.erase(trigger.begin() + j);
						}
						return;
					}
				}
			}
			// Check for object in containers
			for (std::size_t k = 0; k < container.size(); ++k) {
				if (container[k]->name == trigger[j]->condition.status.object) {
					if (container[k]->status == trigger[j]->condition.status.status) {
						std::cout << trigger[j]->print << std::endl;
						for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
							BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
						}
						if (trigger[j]->type == "single") {
							trigger.erase(trigger.begin() + j);
						}
						return;
					}
				}
				for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
					if (container[k]->item[n]->name == trigger[j]->condition.status.object) {
						if (container[k]->item[n]->status == trigger[j]->condition.status.status) {
							std::cout << trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
								BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (trigger[j]->type == "single") {
								trigger.erase(trigger.begin() + j);
							}
							return;
						}
					}
				}
			}
			// Check for object as creature
			for (std::size_t k = 0; k < creature.size(); ++k) {
				if (creature[k]->name == trigger[j]->condition.status.object) {
					if (creature[k]->status == trigger[j]->condition.status.status) {
						std::cout << trigger[j]->print << std::endl;
						for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
							BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
						}
						if (trigger[j]->type == "single") {
							trigger.erase(trigger.begin() + j);
						}
						return;
					}
				}
			}
		}
		
	}
	for (std::size_t z = 0; z < item.size(); ++z) {
		for (std::size_t j = 0; j < item[z]->trigger.size(); ++j) {
			if (item[z]->trigger[j]->command == "") {
				bool has = false;
				//================================================================================================================================================
				// Check OWNER-HAS-OBJECT trigger
				// Check to see if inv is owner and has object
				if (item[z]->trigger[j]->condition.owner.owner == "inventory") {
					for (std::size_t k = 0; k < inv->item.size(); ++k) {
						if (inv->item[k]->name == item[z]->trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check to see if room is owner and has object
				else if (item[z]->trigger[j]->condition.owner.owner == name) {
					for (std::size_t k = 0; k < item.size(); ++k) {
						if (item[k]->name == item[z]->trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check containers in room for owner and check if it has object
				else {
					for (std::size_t k = 0; k < container.size(); ++k) {
						if (container[k]->name == item[z]->trigger[j]->condition.owner.owner) {
							for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
								if (container[k]->item[n]->name == item[z]->trigger[j]->condition.owner.object) {
									has = true;
									break;
								}
							}
							break;
						}
					}
				}
				if ((item[z]->trigger[j]->condition.owner.has == "yes" && has) || (item[z]->trigger[j]->condition.owner.has == "no" && !has)) {
					std::cout << item[z]->trigger[j]->print << std::endl;
					for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
						BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
					}
					if (item[z]->trigger[j]->type == "single") {
						item[z]->trigger.erase(item[z]->trigger.begin() + j);
					}
					return;
				}
				//================================================================================================================================================
				// Check OBJECT-STATUS trigger
				// Check for object in inventory
				std::string status;
				for (std::size_t k = 0; k < inv->item.size(); ++k) {
					if (inv->item[k]->name == item[z]->trigger[j]->condition.status.object) {
						if (inv->item[k]->status == item[z]->trigger[j]->condition.status.status) {
							std::cout << item[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (item[z]->trigger[j]->type == "single") {
								item[z]->trigger.erase(item[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object in room
				for (std::size_t k = 0; k < item.size(); ++k) {
					if (item[k]->name == item[z]->trigger[j]->condition.status.object) {
						if (item[k]->status == item[z]->trigger[j]->condition.status.status) {
							std::cout << item[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (item[z]->trigger[j]->type == "single") {
								item[z]->trigger.erase(item[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object as creature
				for (std::size_t k = 0; k < creature.size(); ++k) {
					if (creature[k]->name == item[z]->trigger[j]->condition.status.object) {
						if (creature[k]->status == item[z]->trigger[j]->condition.status.status) {
							std::cout << item[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (item[z]->trigger[j]->type == "single") {
								item[z]->trigger.erase(item[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object in containers
				for (std::size_t k = 0; k < container.size(); ++k) {
					if (container[k]->name == item[z]->trigger[j]->condition.status.object) {
						if (container[k]->status == item[z]->trigger[j]->condition.status.status) {
							std::cout << item[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (item[z]->trigger[j]->type == "single") {
								item[z]->trigger.erase(item[z]->trigger.begin() + j);
							}
							return;
						}
					}
					for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
						if (container[k]->item[n]->name == item[z]->trigger[j]->condition.status.object) {
							if (container[k]->item[n]->status == item[z]->trigger[j]->condition.status.status) {
								std::cout << item[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (item[z]->trigger[j]->type == "single") {
									item[z]->trigger.erase(item[z]->trigger.begin() + j);
								}
								return;
							}
						}
					}
				}
			}
		}
	}
	for (std::size_t z = 0; z < creature.size(); ++z) {
		for (std::size_t j = 0; j < creature[z]->trigger.size(); ++j) {
			if (creature[z]->trigger[j]->command == "") {
				bool has = false;
				//================================================================================================================================================
				// Check OWNER-HAS-OBJECT trigger
				// Check to see if inv is owner and has object
				if (creature[z]->trigger[j]->condition.owner.owner == "inventory") {
					for (std::size_t k = 0; k < inv->item.size(); ++k) {
						if (inv->item[k]->name == creature[z]->trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check to see if room is owner and has object
				else if (creature[z]->trigger[j]->condition.owner.owner == name) {
					for (std::size_t k = 0; k < item.size(); ++k) {
						if (item[k]->name == creature[z]->trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check containers in room for owner and check if it has object
				else {
					for (std::size_t k = 0; k < container.size(); ++k) {
						if (container[k]->name == creature[z]->trigger[j]->condition.owner.owner) {
							for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
								if (container[k]->item[n]->name == creature[z]->trigger[j]->condition.owner.object) {
									has = true;
									break;
								}
							}
							break;
						}
					}
				}
				if ((creature[z]->trigger[j]->condition.owner.has == "yes" && has) || (creature[z]->trigger[j]->condition.owner.has == "no" && !has)) {
					std::cout << creature[z]->trigger[j]->print << std::endl;
					for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
						BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
					}
					if (creature[z]->trigger[j]->type == "single") {
						creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
					}
					return;
				}
				//================================================================================================================================================
				// Check OBJECT-STATUS trigger
				// Check for object in inventory
				std::string status;
				for (std::size_t k = 0; k < inv->item.size(); ++k) {
					if (inv->item[k]->name == creature[z]->trigger[j]->condition.status.object) {
						if (inv->item[k]->status == creature[z]->trigger[j]->condition.status.status) {
							std::cout << creature[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (creature[z]->trigger[j]->type == "single") {
								creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object in room
				for (std::size_t k = 0; k < item.size(); ++k) {
					if (item[k]->name == creature[z]->trigger[j]->condition.status.object) {
						if (item[k]->status == creature[z]->trigger[j]->condition.status.status) {
							std::cout << creature[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (creature[z]->trigger[j]->type == "single") {
								creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object as creature
				for (std::size_t k = 0; k < creature.size(); ++k) {
					if (creature[k]->name == creature[z]->trigger[j]->condition.status.object) {
						if (creature[k]->status == creature[z]->trigger[j]->condition.status.status) {
							std::cout << creature[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (creature[z]->trigger[j]->type == "single") {
								creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object in containers
				for (std::size_t k = 0; k < container.size(); ++k) {
					if (container[k]->name == creature[z]->trigger[j]->condition.status.object) {
						if (container[k]->status == creature[z]->trigger[j]->condition.status.status) {
							std::cout << creature[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (creature[z]->trigger[j]->type == "single") {
								creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
							}
							return;
						}
						for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
							if (container[k]->item[n]->name == creature[z]->trigger[j]->condition.status.object) {
								if (container[k]->item[n]->status == creature[z]->trigger[j]->condition.status.status) {
									std::cout << creature[z]->trigger[j]->print << std::endl;
									for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
										BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
									}
									if (creature[z]->trigger[j]->type == "single") {
										creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
									}
									return;
								}
							}
						}
					}
				}
			}
		}
	}
	for (std::size_t z = 0; z < container.size(); ++z) {
		for (std::size_t j = 0; j < container[z]->trigger.size(); ++j) {
			if (container[z]->trigger[j]->command == "") {
				bool has = false;
				//================================================================================================================================================
				// Check OWNER-HAS-OBJECT trigger
				// Check to see if inv is owner and has object
				if (container[z]->trigger[j]->condition.owner.owner == "inventory") {
					for (std::size_t k = 0; k < inv->item.size(); ++k) {
						if (inv->item[k]->name == container[z]->trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check to see if room is owner and has object
				else if (container[z]->trigger[j]->condition.owner.owner == name) {
					for (std::size_t k = 0; k < item.size(); ++k) {
						if (item[k]->name == container[z]->trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check containers in room for owner and check if it has object
				else {
					for (std::size_t k = 0; k < container.size(); ++k) {
						if (container[k]->name == container[z]->trigger[j]->condition.owner.owner) {
							for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
								if (container[k]->item[n]->name == container[z]->trigger[j]->condition.owner.object) {
									has = true;
									break;
								}
							}
							break;
						}
					}
				}
				if ((container[z]->trigger[j]->condition.owner.has == "yes" && has) || (container[z]->trigger[j]->condition.owner.has == "no" && !has)) {
					std::cout << container[z]->trigger[j]->print << std::endl;
					for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
						BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
					}
					if (container[z]->trigger[j]->type == "single") {
						container[z]->trigger.erase(container[z]->trigger.begin() + j);
					}
					return;
				}

				//================================================================================================================================================
				// Check OBJECT-STATUS trigger
				// Check for object in inventory
				std::string status;
				for (std::size_t k = 0; k < inv->item.size(); ++k) {
					if (inv->item[k]->name == container[z]->trigger[j]->condition.status.object) {
						if (inv->item[k]->status == container[z]->trigger[j]->condition.status.status) {
							std::cout << container[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (container[z]->trigger[j]->type == "single") {
								container[z]->trigger.erase(container[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}

				// Check for object in room
				for (std::size_t k = 0; k < item.size(); ++k) {
					if (item[k]->name == container[z]->trigger[j]->condition.status.object) {
						if (item[k]->status == container[z]->trigger[j]->condition.status.status) {
							std::cout << container[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (container[z]->trigger[j]->type == "single") {
								container[z]->trigger.erase(container[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object as creature
				for (std::size_t k = 0; k < creature.size(); ++k) {
					if (creature[k]->name == container[z]->trigger[j]->condition.status.object) {
						if (creature[k]->status == container[z]->trigger[j]->condition.status.status) {
							std::cout << container[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (container[z]->trigger[j]->type == "single") {
								container[z]->trigger.erase(container[z]->trigger.begin() + j);
							}
							return;
						}
					}
				}
				// Check for object in containers
				for (std::size_t k = 0; k < container.size(); ++k) {
					if (container[k]->name == container[z]->trigger[j]->condition.status.object) {
						if (container[k]->status == container[z]->trigger[j]->condition.status.status) {
							std::cout << container[z]->trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
								BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (container[z]->trigger[j]->type == "single") {
								container[z]->trigger.erase(container[z]->trigger.begin() + j);
							}
							return;
						}
					}
					for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
						if (container[k]->item[n]->name == container[z]->trigger[j]->condition.status.object) {
							if (container[k]->item[n]->status == container[z]->trigger[j]->condition.status.status) {
								std::cout << container[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (container[z]->trigger[j]->type == "single") {
									container[z]->trigger.erase(container[z]->trigger.begin() + j);
								}
								return;
							}
						}
					}
				}
			}
		}
	}
	
	return;
}


int Room::ActivateTrigger(std::string cmd, Inventory* inv, std::vector<Item*> i, std::vector<Container*> c, std::vector<Creature*> creeps, std::vector<Room*> r) {
	for (std::size_t j = 0; j < trigger.size(); ++j) {
		if (trigger[j]->command == cmd) {
			//================================================================================================================================================
			// Check OWNER-HAS-OBJECT trigger
			if (trigger[j]->condition.owner.owner != "") {
				bool has = false;
				// Check to see if inv is owner and has object
				if (trigger[j]->condition.owner.owner == "inventory") {
					for (std::size_t k = 0; k < inv->item.size(); ++k) {
						if (inv->item[k]->name == trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check to see if room is owner and has object
				else if (trigger[j]->condition.owner.owner == name) {
					for (std::size_t k = 0; k < item.size(); ++k) {
						if (item[k]->name == trigger[j]->condition.owner.object) {
							has = true;
							break;
						}
					}
				}
				// Check containers in room for owner and check if it has object
				else {
					for (std::size_t k = 0; k < container.size(); ++k) {
						if (container[k]->name == trigger[j]->condition.owner.owner) {
							for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
								if (container[k]->item[n]->name == trigger[j]->condition.owner.object) {
									has = true;
									break;
								}
							}
							break;
						}
					}
				}
				if ((trigger[j]->condition.owner.has == "yes" && has) || (trigger[j]->condition.owner.has == "no" && !has)) {
					std::cout << trigger[j]->print << std::endl;
					for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
						BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
					}
					if (trigger[j]->type == "single") {
						trigger.erase(trigger.begin() + j);
					}
					return 1;
				}
				else {
					return 2;
				}
			}
			//================================================================================================================================================
			// Check OBJECT-STATUS trigger
			else if (trigger[j]->condition.status.status != "") {
				std::string status;
				for (std::size_t k = 0; k < inv->item.size(); ++k) {
					if (inv->item[k]->name == trigger[j]->condition.status.object) {
						if (inv->item[k]->status == trigger[j]->condition.status.status) {
							std::cout << trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
								BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (trigger[j]->type == "single") {
								trigger.erase(trigger.begin() + j);
							}
							return 1;
						}
						else {
							return 2;
						}
					}
				}

				// Check for object in room
				for (std::size_t k = 0; k < item.size(); ++k) {
					if (item[k]->name == trigger[j]->condition.status.object) {
						if (item[k]->status == trigger[j]->condition.status.status) {
							std::cout << trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
								BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (trigger[j]->type == "single") {
								trigger.erase(trigger.begin() + j);
							}
							return 1;
						}
						else {
							return 2;
						}
					}
				}
				// Check for object as creature
				for (std::size_t k = 0; k < creature.size(); ++k) {
					if (creature[k]->name == trigger[j]->condition.status.object) {
						if (creature[k]->status == trigger[j]->condition.status.status) {
							std::cout << trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
								BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (trigger[j]->type == "single") {
								trigger.erase(trigger.begin() + j);
							}
							return 1;
						}
						else {
							return 2;
						}
					}
				}
				// Check for object in containers
				for (std::size_t k = 0; k < container.size(); ++k) {
					if (container[k]->name == trigger[j]->condition.status.object) {
						if (container[k]->status == trigger[j]->condition.status.status) {
							std::cout << trigger[j]->print << std::endl;
							for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
								BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
							}
							if (trigger[j]->type == "single") {
								trigger.erase(trigger.begin() + j);
							}
							return 1;
						}
						else {
							return 2;
						}
					}
					for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
						if (container[k]->item[n]->name == trigger[j]->condition.status.object) {
							if (container[k]->item[n]->status == trigger[j]->condition.status.status) {
								std::cout << trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < trigger[j]->action.size(); ++k) {
									BehindTheScenes(trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (trigger[j]->type == "single") {
									trigger.erase(trigger.begin() + j);
								}
								return 1;
							}
							else {
								return 2;
							}
						}
					}
				}
			}
			// Check for object in inventory
			
		}
	}
	for (std::size_t z = 0; z < item.size(); ++z) {
		for (std::size_t j = 0; j < item[z]->trigger.size(); ++j) {
			if (item[z]->trigger[j]->command == cmd) {
				//================================================================================================================================================
				// Check OWNER-HAS-OBJECT trigger
				if (item[z]->trigger[j]->condition.owner.owner != "") {
					bool has = false;
					// Check to see if inv is owner and has object
					if (item[z]->trigger[j]->condition.owner.owner == "inventory") {
						for (std::size_t k = 0; k < inv->item.size(); ++k) {
							if (inv->item[k]->name == item[z]->trigger[j]->condition.owner.object) {
								has = true;
								break;
							}
						}
					}
					// Check to see if room is owner and has object
					else if (item[z]->trigger[j]->condition.owner.owner == name) {
						for (std::size_t k = 0; k < item.size(); ++k) {
							if (item[k]->name == item[z]->trigger[j]->condition.owner.object) {
								has = true;
								break;
							}
						}
					}
					// Check containers in room for owner and check if it has object
					else {
						for (std::size_t k = 0; k < container.size(); ++k) {
							if (container[k]->name == item[z]->trigger[j]->condition.owner.owner) {
								for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
									if (container[k]->item[n]->name == item[z]->trigger[j]->condition.owner.object) {
										has = true;
										break;
									}
								}
								break;
							}
						}
					}
					if ((item[z]->trigger[j]->condition.owner.has == "yes" && has) || (item[z]->trigger[j]->condition.owner.has == "no" && !has)) {
						std::cout << item[z]->trigger[j]->print << std::endl;
						for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
							BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
						}
						if (item[z]->trigger[j]->type == "single") {
							item[z]->trigger.erase(item[z]->trigger.begin() + j);
						}
						return 1;
					}
					else {
						return 2;
					}
				}
				//================================================================================================================================================
				// Check OBJECT-STATUS trigger
				else if (item[z]->trigger[j]->condition.status.status != "") {
					std::string status;
					for (std::size_t k = 0; k < inv->item.size(); ++k) {
						if (inv->item[k]->name == item[z]->trigger[j]->condition.status.object) {
							if (inv->item[k]->status == item[z]->trigger[j]->condition.status.status) {
								std::cout << item[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (item[z]->trigger[j]->type == "single") {
									item[z]->trigger.erase(item[z]->trigger.begin() + j);
								}
								return 1;
							}
							else {
								return 2;
							}
						}
					}

					// Check for object in room
					for (std::size_t k = 0; k < item.size(); ++k) {
						if (item[k]->name == item[z]->trigger[j]->condition.status.object) {
							if (item[k]->status == item[z]->trigger[j]->condition.status.status) {
								std::cout << item[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (item[z]->trigger[j]->type == "single") {
									item[z]->trigger.erase(item[z]->trigger.begin() + j);
								}
								return 1;
							}
							else {
								return 2;
							}
						}
					}
					// Check for object in containers
					for (std::size_t k = 0; k < container.size(); ++k) {
						for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
							if (container[k]->item[n]->name == item[z]->trigger[j]->condition.owner.object) {
								if (container[k]->item[n]->status == item[z]->trigger[j]->condition.status.status) {
									std::cout << item[z]->trigger[j]->print << std::endl;
									for (std::size_t k = 0; k < item[z]->trigger[j]->action.size(); ++k) {
										BehindTheScenes(item[z]->trigger[j]->action[k], i, c, creeps, r, inv);
									}
									if (item[z]->trigger[j]->type == "single") {
										item[z]->trigger.erase(item[z]->trigger.begin() + j);
									}
									return 1;
								}
								else {
									return 2;
								}
							}
						}
					}
				}			
			}
		}
	}
	for (std::size_t z = 0; z < creature.size(); ++z) {
		for (std::size_t j = 0; j < creature[z]->trigger.size(); ++j) {
			if (creature[z]->trigger[j]->command == cmd) {
				//================================================================================================================================================
				// Check OWNER-HAS-OBJECT trigger
				if (creature[z]->trigger[j]->condition.owner.owner != "") {
					bool has = false;
					// Check to see if inv is owner and has object
					if (creature[z]->trigger[j]->condition.owner.owner == "inventory") {
						for (std::size_t k = 0; k < inv->item.size(); ++k) {
							if (inv->item[k]->name == creature[z]->trigger[j]->condition.owner.object) {
								has = true;
								break;
							}
						}
					}
					// Check to see if room is owner and has object
					else if (creature[z]->trigger[j]->condition.owner.owner == name) {
						for (std::size_t k = 0; k < item.size(); ++k) {
							if (item[k]->name == creature[z]->trigger[j]->condition.owner.object) {
								has = true;
								break;
							}
						}
					}
					// Check containers in room for owner and check if it has object
					else {
						for (std::size_t k = 0; k < container.size(); ++k) {
							if (container[k]->name == creature[z]->trigger[j]->condition.owner.owner) {
								for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
									if (container[k]->item[n]->name == creature[z]->trigger[j]->condition.owner.object) {
										has = true;
										break;
									}
								}
								break;
							}
						}
					}
					if ((creature[z]->trigger[j]->condition.owner.has == "yes" && has) || (creature[z]->trigger[j]->condition.owner.has == "no" && !has)) {
						std::cout << creature[z]->trigger[j]->print << std::endl;
						for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
							BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
						}
						if (creature[z]->trigger[j]->type == "single") {
							creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
						}
						return 1;
					}
					else {
						return 2;
					}
				}
				//================================================================================================================================================
				// Check OBJECT-STATUS trigger
				else if (creature[z]->trigger[j]->condition.status.status != "") {
					// Check for object in inventory
					std::string status;
					for (std::size_t k = 0; k < inv->item.size(); ++k) {
						if (inv->item[k]->name == creature[z]->trigger[j]->condition.status.object) {
							if (inv->item[k]->status == creature[z]->trigger[j]->condition.status.status) {
								std::cout << creature[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (creature[z]->trigger[j]->type == "single") {
									creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
								}
								return 1;
							}
							else {
								return 2;
							}
						}
					}

					// Check for object in room
					for (std::size_t k = 0; k < item.size(); ++k) {
						if (item[k]->name == creature[z]->trigger[j]->condition.status.object) {
							if (item[k]->status == creature[z]->trigger[j]->condition.status.status) {
								std::cout << creature[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (creature[z]->trigger[j]->type == "single") {
									creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
								}
								return 1;
							}
							else {
								return 2;
							}
						}
					}
					// Check for object in containers
					for (std::size_t k = 0; k < container.size(); ++k) {
						for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
							if (container[k]->item[n]->name == creature[z]->trigger[j]->condition.owner.object) {
								if (container[k]->item[n]->status == creature[z]->trigger[j]->condition.status.status) {
									std::cout << creature[z]->trigger[j]->print << std::endl;
									for (std::size_t k = 0; k < creature[z]->trigger[j]->action.size(); ++k) {
										BehindTheScenes(creature[z]->trigger[j]->action[k], i, c, creeps, r, inv);
									}
									if (creature[z]->trigger[j]->type == "single") {
										creature[z]->trigger.erase(creature[z]->trigger.begin() + j);
									}
									return 1;
								}
								else {
									return 2;
								}
							}
						}
					}
				}
				
			}
		}
	}
	for (std::size_t z = 0; z < container.size(); ++z) {
		for (std::size_t j = 0; j < container[z]->trigger.size(); ++j) {
			if (container[z]->trigger[j]->command == cmd) {
				//================================================================================================================================================
				// Check OWNER-HAS-OBJECT trigger
				if (container[z]->trigger[j]->condition.owner.owner != "") {
					bool has = false;
					// Check to see if inv is owner and has object
					if (container[z]->trigger[j]->condition.owner.owner == "inventory") {
						for (std::size_t k = 0; k < inv->item.size(); ++k) {
							if (inv->item[k]->name == container[z]->trigger[j]->condition.owner.object) {
								has = true;
								break;
							}
						}
					}
					// Check to see if room is owner and has object
					else if (container[z]->trigger[j]->condition.owner.owner == name) {
						for (std::size_t k = 0; k < item.size(); ++k) {
							if (item[k]->name == container[z]->trigger[j]->condition.owner.object) {
								has = true;
								break;
							}
						}
					}
					// Check containers in room for owner and check if it has object
					else {
						for (std::size_t k = 0; k < container.size(); ++k) {
							if (container[k]->name == container[z]->trigger[j]->condition.owner.owner) {
								for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
									if (container[k]->item[n]->name == container[z]->trigger[j]->condition.owner.object) {
										has = true;
										break;
									}
								}
								break;
							}
						}
					}
					if ((container[z]->trigger[j]->condition.owner.has == "yes" && has) || (container[z]->trigger[j]->condition.owner.has == "no" && !has)) {
						std::cout << container[z]->trigger[j]->print << std::endl;
						for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
							BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
						}
						if (container[z]->trigger[j]->type == "single") {
							container[z]->trigger.erase(container[z]->trigger.begin() + j);
						}
						return 1;
					}
					else {
						return 2;
					}
				}
				//================================================================================================================================================
				// Check OBJECT-STATUS trigger
				else if (container[z]->trigger[j]->condition.status.status != "") {
					std::string status;
					for (std::size_t k = 0; k < inv->item.size(); ++k) {
						if (inv->item[k]->name == container[z]->trigger[j]->condition.status.object) {
							if (inv->item[k]->status == container[z]->trigger[j]->condition.status.status) {
								std::cout << container[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (container[z]->trigger[j]->type == "single") {
									container[z]->trigger.erase(container[z]->trigger.begin() + j);
								}
								return 1;
							}
							else {
								return 2;
							}
						}
					}
					// Check for object in room
					for (std::size_t k = 0; k < item.size(); ++k) {
						if (item[k]->name == container[z]->trigger[j]->condition.status.object) {
							if (item[k]->status == container[z]->trigger[j]->condition.status.status) {
								std::cout << container[z]->trigger[j]->print << std::endl;
								for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
									BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
								}
								if (container[z]->trigger[j]->type == "single") {
									container[z]->trigger.erase(container[z]->trigger.begin() + j);
								}
								return 1;
							}
							else {
								return 2;
							}
						}
					}
					// Check for object in containers
					for (std::size_t k = 0; k < container.size(); ++k) {
						for (std::size_t n = 0; n < container[k]->item.size(); ++n) {
							if (container[k]->item[n]->name == container[z]->trigger[j]->condition.owner.object) {
								if (container[k]->item[n]->status == container[z]->trigger[j]->condition.status.status) {
									std::cout << container[z]->trigger[j]->print << std::endl;
									for (std::size_t k = 0; k < container[z]->trigger[j]->action.size(); ++k) {
										BehindTheScenes(container[z]->trigger[j]->action[k], i, c, creeps, r, inv);
									}
									if (container[z]->trigger[j]->type == "single") {
										container[z]->trigger.erase(container[z]->trigger.begin() + j);
									}
									return 1;
								}
								else {
									return 2;
								}
							}
						}
					}
				}
				// Check for object in inventory
			}
		}
	}
	return 0;
}


void Room::BehindTheScenes(std::string command, std::vector<Item*> i, std::vector<Container*> c, std::vector<Creature*> creeps, std::vector<Room*> r, Inventory* inv) {
	std::string obj;
	std::string whatToDo;
	std::string useless;
	std::string holderCommand;
	std::string objectCommand;
	std::string statusCommand;
	std::string moveRoom;
	std::stringstream action(command);
	Item* foundItem = NULL;
	Creature* foundCreature = NULL;
	Container* foundContainer = NULL;
	bool foundI = false;
	bool foundCreep = false;
	bool foundC = false;
	action >> whatToDo;
	if (whatToDo == "Add") {
		action >> objectCommand;
		for (std::size_t j = 0; j < i.size(); ++j) {
			if (i[j]->name == objectCommand) {
				foundItem = i[j];
				foundI = true;
				break;
			}
		}
		if (!foundI) {
			for (std::size_t j = 0; j < creeps.size(); ++j) {
				if (creeps[j]->name == objectCommand) {
					foundCreature = creeps[j];
					foundCreep = true;
					break;
				}
			}
		}
		if (!foundI && !foundCreep) {
			for (std::size_t j = 0; j < c.size(); ++j) {
				if (c[j]->name == objectCommand) {
					foundContainer = c[j];
					foundC;
					break;
				}
			}
		}
		action >> useless;
		action >> holderCommand;
		if (holderCommand == name) { // Assuming can only add to current room
			if (foundI) {
				Add(foundItem);
			}
			else if (foundCreep) {
				creature.push_back(foundCreature);
			}
			else if (foundC) {
				container.push_back(foundContainer);
			}
		}
		else {
			for (std::size_t j = 0; j < c.size(); ++j) {
				if (c[j]->name == holderCommand) {
					if (foundI) {
						c[j]->Add(foundItem);
					}
				}
			}
		}
	}
	else if (command == "Game Over") {
		std::cout << "Victory!" << std::endl;
		GAME = false;
	}
	else if (whatToDo == "Update") {
		action >> objectCommand;
		action >> useless;
		action >> statusCommand;
		for (std::size_t j = 0; j < inv->item.size(); ++j) {
			if (inv->item[j]->name == objectCommand) {
				inv->item[j]->status = statusCommand;
				foundI = true;
				break;
			}
		}
		if(!foundI){
			for (std::size_t j = 0; j < r.size(); ++j) {
				if (r[j]->name == objectCommand) {
					r[j]->status = statusCommand;
					break;
				}
				for (std::size_t k = 0; k < r[j]->item.size(); ++k) {
					if (r[j]->item[k]->name == objectCommand) {
						r[j]->item[k]->status = statusCommand;
						break;
					}
				}
				for (std::size_t k = 0; k < r[j]->creature.size(); ++k) {
					if (r[j]->creature[k]->name == objectCommand) {
						r[j]->creature[k]->status = statusCommand;
						break;
					}
				}
				for (std::size_t k = 0; k < r[j]->container.size(); ++k) {
					if (r[j]->container[k]->name == objectCommand) {
						r[j]->container[k]->status = statusCommand;
						break;
					}
				}
			}
		}
	}
	else if (whatToDo == "Delete") {
		action >> objectCommand;
		for (std::size_t j = 0; j < boarder.size(); ++j) {
			if (boarder[j]->name == objectCommand) {
				boarder.erase(boarder.begin() + j);
			}
		}
		for (std::size_t j = 0; j < item.size(); ++j) {
			if (item[j]->name == objectCommand) {
				item.erase(item.begin() + j);
			}
		}
		for (std::size_t j = 0; j < creature.size(); ++j) {
			if (creature[j]->name == objectCommand) {
				creature.erase(creature.begin() + j);
			}
		}
		for (std::size_t j = 0; j < container.size(); ++j) {
			if (container[j]->name == objectCommand) {
				container.erase(container.begin() + j);
			}
		}
		for (std::size_t j = 0; j < inv->item.size(); ++j) {
			if (inv->item[j]->name == objectCommand) {
				inv->item.erase(inv->item.begin() + j);
			}
		}
	}
	else if (whatToDo == "drop") {
		action >> objectCommand;
		if (inv->Have(objectCommand)) {
			Item* found = inv->Drop(objectCommand);
			Add(found);
		}
	}
	else if (whatToDo == "take") {
		action >> objectCommand;
		if (Have(objectCommand)) {
			Item* found = GetItem(objectCommand);
			ClearContainer(objectCommand);
			inv->Put(found);
		}
	}
	else if (whatToDo == "n") {
		if (HaveBoarder("north")) {
			moveRoom = Move("north");
			for (std::size_t j = 0; j < r.size(); ++j) {
				if (r[j]->name == moveRoom) {
					currRoom = r[j];
					break;
				}
			}
			currRoom->PlayIntro();
		}
	}
	else if (whatToDo == "s") {
		if (currRoom->HaveBoarder("south")) {
			moveRoom = currRoom->Move("south");
			for (std::size_t j = 0; j < r.size(); ++j) {
				if (r[j]->name == moveRoom) {
					currRoom = r[j];
					break;
				}
			}
			currRoom->PlayIntro();
		}
	}
	else if (whatToDo == "e") {
		if (currRoom->HaveBoarder("east")) {
			moveRoom = currRoom->Move("east");
			for (std::size_t j = 0; j < r.size(); ++j) {
				if (r[j]->name == moveRoom) {
					currRoom = r[j];
					break;
				}
			}
			currRoom->PlayIntro();
		}
	}
	else if (whatToDo == "w") {
		if (currRoom->HaveBoarder("west")) {
			moveRoom = currRoom->Move("west");
			for (std::size_t j = 0; j < r.size(); ++j) {
				if (r[j]->name == moveRoom) {
					currRoom = r[j];
					break;
				}
			}
			currRoom->PlayIntro();
		}
	}
}

void Room::AddCreatures(std::vector<Creature*> c) {
	for (std::size_t j = 0; j < creatureList.size(); ++j) {
		for (std::size_t k = 0; k < c.size(); ++k) {
			if (creatureList[j] == c[k]->name) {
				creature.push_back(c[k]);
			}
		}
	}
}

void Room::AddContainers(std::vector<Container*> c) {
	for (std::size_t j = 0; j < containerList.size(); ++j) {
		for (std::size_t k = 0; k < c.size(); ++k) {
			if (containerList[j] == c[k]->name) {
				container.push_back(c[k]);
			}
		}
	}
}

void Room::SetType(std::string t) {
	type = t;
}

void Room::AttackCreature(std::string c, std::string i, Inventory* inv, std::vector<Item*> it, std::vector<Container*> con, std::vector<Creature*> cre, std::vector<Room*> r) {
	Item* weapon = NULL;
	for (std::size_t j = 0; j < inv->item.size(); ++j) {
		if (inv->item[j]->name == i) {
			weapon = inv->item[j];
			break;
		}
	}
	for (std::size_t j = 0; j < creature.size(); ++j) {
		if (creature[j]->name == c) {
			if (AttackWithWeapon(c, weapon, creature[j], inv, it, con, cre, r)) {
				return;
			}
			std::cout << "You Assult the " << c << " with the " << weapon->name << std::endl;
			return;
		}
	}
}

bool Room::AttackWithWeapon(std::string c, Item* i, Creature* cre, Inventory* inv, std::vector<Item*> it, std::vector<Container*> con, std::vector<Creature*> creeps, std::vector<Room*> r) {
	for (std::size_t j = 0; j < cre->vulnerability.size(); ++j) {
		if (cre->vulnerability[j] == i->name) {
			if (cre->attack->condition.status.status == i->status) {
				std::cout << "You Assult the " << c << " with the " << i->name << std::endl;
				std::cout << cre->attack->print << std::endl;
				for (std::size_t k = 0; k < cre->attack->action.size(); ++k) {
					BehindTheScenes(cre->attack->action[k], it, con, creeps, r, inv);
				}
				return true;
			}
		}
	}
	return false;
}

void Room::TurnOnItem(std::string itemName, std::vector<Item*> things, std::vector<Container*> c, std::vector<Creature*> creeps, std::vector<Room*> r, Inventory* inv){
	Item* tmpItem = NULL;
	for (std::size_t j = 0; j < inv->item.size(); j++) {
		if (inv->item[j]->name == itemName) {
			tmpItem = inv->item[j];
		}
	}
	tmpItem->TurnOn();
	for (std::size_t j = 0; j < tmpItem->action.size(); ++j) {
		BehindTheScenes(tmpItem->action[j], things, c, creeps, r, inv);
	}
}