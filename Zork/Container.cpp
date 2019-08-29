#include "Container.h"

Container::Container(){}

Container::~Container(){}

std::vector<Item*> Container::ClearContents() {
	std::vector<Item*> tmp;
	for (std::size_t j = 0; j < item.size(); ++j) {
		tmp.push_back(item[j]);
		item.erase(item.begin() + j);
	}
	return tmp;
}

void Container::PrintContents() {
	if (itemList.size() == 0) {
		std::cout << name << " is empty." << std::endl;
	}
	else {
		std::cout << name << " contains";
		for (std::size_t i = 0; i < itemList.size(); ++i) {
			std::cout << " " << itemList[i];
		}
		std::cout << std::endl;
	}
}

void Container::AddAccept(std::string a) {
	accept.push_back(a);
}

bool Container::CheckAcceptance(std::string i) {
	if (accept.size() == 0) {
		return true;
	}
	else {
		for (std::size_t j = 0; j < accept.size(); ++j) {
			if (accept[j] == i) {
				return true;
			}
		}
		return false;
	}
}