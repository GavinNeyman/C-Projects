#ifndef BOARDER_H
#define BOARDER_H
#include <string>
class Boarder{
public:
	Boarder();
	~Boarder();
	void SetDirection(std::string);
	void SetName(std::string);
	//============================================================================================================================================================
	// Variables
	std::string direction;
	std::string name;
};

#endif

