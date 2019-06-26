#pragma once
#include <string>
#include "Octree.h"
#include "base.hpp"
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

