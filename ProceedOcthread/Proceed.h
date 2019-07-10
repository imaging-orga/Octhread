#pragma once
#include <string>
#include "../Octhread/Octree.h"
#include "../Octhread/base.hpp"


namespace FileOct{
	//Get the number of points that is contained in particular file (read in .OcSave)
	long int findNumPointsFromFile(std::string _fileptsName, std::string _name);
	BoundingBox findBBFromFile(std::string _fileptsName, std::string _name);

	//Create a vector from the points contained in _name;
	std::vector<mypt3d> getPtsFromFile(std::string _fileptsName, std::string _name);
	long int getMaxPoints(std::string _fileptsName);
	std::vector<std::string> nameOfNon0files(std::string  _fileptsName);
}