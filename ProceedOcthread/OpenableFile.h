#pragma once
#include <string>
#include "../Octhread/Octree.h"
#include "../Octhread/base.hpp"
class OpenableFile
{
public:
	OpenableFile(std::string name);
	virtual BoundingBox getBoundingBox() = 0;
	virtual void read() = 0;


protected : 
	std::string p_name;
	Octree p_oct;
};

