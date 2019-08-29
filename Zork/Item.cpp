#include "Item.h"

Item::Item(){
	writing = "nothing written.";
	print = "";
}

Item::~Item(){
}

std::string Item::GetName() {
	return name;
}

void Item::TurnOn() {
	std::cout << "You activate the " << name << std::endl;
	std::cout << print << std::endl;
}

void Item::AddAction(std::string a) {
	action.push_back(a);
}

void Item::SetWriting(std::string w) {
	writing = w;
}
void Item::SetPrint(std::string p) {
	print = p;
}