#pragma once
#include <string>
#include "base.hpp"
#include <vector>

#include <fstream>
class MyFile
{
public:
	MyFile(std::string filename);
	~MyFile();

	//create file if it doesn't exists yet
	void createFile();
	
	//read the points from the file to a new vector
	std::vector<std::shared_ptr<mypt3d>> readFromFile(int numPoints);

	//write points of vectors in the file
	void writeToFile(std::vector<std::shared_ptr<mypt3d>>& pts);



private:
	std::string m_filename;
};

