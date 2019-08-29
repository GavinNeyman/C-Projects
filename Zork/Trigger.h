#ifndef TRIGGER_H
#define TRIGGER_H
#include <string>
#include <vector>
class Trigger
{
public:
	Trigger();
	~Trigger();
	
	void AddAction(std::string);
	void AddPrint(std::string);
	void SetCommand(std::string);
	void SetType(std::string);
	void SetObject(std::string);
	void SetOwner(std::string);
	void SetHas(std::string);
	void SetStatus(std::string);

	std::string command;
	std::string type;
	std::string print;
	std::vector<std::string> action;
	struct Owner {
		std::string object;
		std::string has;
		std::string owner;
	};
	struct Status {
		std::string object;
		std::string status;
	};
	struct Condition {
		Owner owner;
		Status status;
	};
	
	Condition condition;
};

#endif