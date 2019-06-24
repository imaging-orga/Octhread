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
	vecPoints readFromFile(int numPoints);

	//write points of vectors in the file
	void writeToFile(vecPoints& pts);



private:
	std::string m_filename;
};

