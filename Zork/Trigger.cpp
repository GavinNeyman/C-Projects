#include "Trigger.h"

Trigger::Trigger(){
	type = "single";
	condition.status.status = "";
	print = "";
}

Trigger::~Trigger(){
}

void Trigger::AddAction(std::string a) {
	action.push_back(a);
}
void Trigger::AddPrint(std::string p) {
	print = p;
}

void Trigger::SetCommand(std::string c) {
	command = c;
}
void Trigger::SetType(std::string t) {
	type = t;
}

void Trigger::SetObject(std::string o) {
	if (condition.owner.has != "") {
		condition.owner.object = o;
	}
	else {
		condition.status.object = o;
	}
}

void Trigger::SetOwner(std::string o) {
	condition.owner.owner = o;
}

void Trigger::SetHas(std::string h) {
	condition.owner.has = h;
}
void Trigger::SetStatus(std::string s) {
	condition.status.status = s;
}
