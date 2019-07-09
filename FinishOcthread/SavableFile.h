#pragma once
#include <string>
#include "../Octhread/base.hpp"
class SavableFile
{
public:
	SavableFile(std::string _name, long int _numMax, BoundingBox& _bb);
	virtual int write(std::vector<mypt3d>& pts) = 0;
	virtual int writeHeader() = 0;
	virtual int writeFooter() = 0;
	~SavableFile();

protected:
	std::string p_name;
	long int p_numMax;
	BoundingBox p_bb;
};



