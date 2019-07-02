#pragma once
#include <string>

class SavableFile
{
public:
	SavableFile(std::string _name);
	virtual void write() = 0;
	virtual void writeHeader() = 0;
	~SavableFile();

protected:
	std::string p_name;
};





/*
SavableFiles sf(trucMuch.e57);
sf->write 
	

*/