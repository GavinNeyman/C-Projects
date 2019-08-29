#include "Holder.h"

Holder::Holder(){}


Holder::~Holder(){}

void Holder::Add(Item* i) {
	item.push_back(i);
}

void Holder::Delete(Item* i) {
	for (std::size_t j = 0; j < item.size(); ++j) {
		if (item[j]->name == i->name) {
			item.erase(item.begin() + j);
			return;
		}
	}
}

void Holder::Put(Item* i) {
	Add(i);
	std::cout << "Item " << i->name << " added to " << name << std::endl;
}

bool Holder::Have(std::string i) {
	for (std::size_t j = 0; j < item.size(); ++j) {
		if (item[j]->name == i) {
			return true;
		}
	}
	std::cout << "Error" << std::endl;
	return false;
}

Item* Holder::GetItem(std::string i) {
	for (std::size_t j = 0; j < item.size(); ++j) {
		if (item[j]->name == i) {
			Item* found = item[j];
			Delete(item[j]);
			return found;
		}
	}
}

void Holder::PushItem(std::string i) {
	itemList.push_back(i);
}

void Holder::AddItems(std::vector<Item*> i) {
	for (std::size_t j = 0; j < itemList.size(); ++j) {
		for (std::size_t k = 0; k < i.size(); ++k) {
			if (itemList[j] == i[k]->name) {
				Add(i[k]);
				break;
			}
		}
	}
}