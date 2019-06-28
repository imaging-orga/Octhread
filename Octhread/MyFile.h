#pragma once
#include <string>
#include "base.hpp"
#include <vector>

#include <fstream>
#include <boost/ptr_container/ptr_vector.hpp>
class MyFile
{
public:
	MyFile();
	MyFile(std::string filename);
	~MyFile();

	//create file if it doesn't exists yet
	void createFile();
	
	//read the points from the file to a new vector
	std::vector<mypt3d> readFromFile(int numPoints);

	//write points of vectors in the file
	void writeToFile(std::vector<mypt3d>& pts);

	// void the file
	void emptyFile();

private:
	std::string m_filename;
};

