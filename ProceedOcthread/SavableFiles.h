#pragma once
#include <string>
#include "boost/filesystem.hpp"
#include <locale>
#include <cstdio>
#include "../Octhread/base.hpp"
enum SAVETYPE {
	T_E57, 
	T_PTX
};

class SavableFiles
{
public:
	SavableFiles(std::string _name);
	~SavableFiles();
	void finishWriting();
	void write(std::vector<mypt3d>& pts);

private:
	std::string m_name;
	std::string m_TMPname;
	SAVETYPE m_type;

	long int numPoints;
	int writeE57(std::vector<mypt3d>& pts);


	void writeHeader();
	int writePTX(std::vector<mypt3d>& pts);


};

