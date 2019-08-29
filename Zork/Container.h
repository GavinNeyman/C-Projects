#ifndef CONTAINER_H
#define CONTAINER_H
#include "Entity.h"
#include "Holder.h"
class Container : public Holder {
public:
	Container();
	~Container();
	//============================================================================================================================================================
	// Functions
	std::vector<Item*> ClearContents();
	void PrintContents();
	void AddAccept(std::string);
	bool CheckAcceptance(std::string);
	//============================================================================================================================================================
	// Variables
	std::vector<std::string> accept;
};
#endif
