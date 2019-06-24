#include "MyFile.h"



MyFile::MyFile(std::string filename_) : m_filename(filename_)
{
}


MyFile::~MyFile()
{
}

void MyFile::createFile()
{
	std::ofstream createFile(m_filename, std::ios::out);
	createFile.close();
}

vecPoints MyFile::readFromFile(int numPoints)
{
	std::ifstream dataFile;
	vecPoints ptsRet;

	dataFile.open(m_filename, std::ios::in | std::ios::binary);
	ptsRet.resize(numPoints);

	dataFile.read(reinterpret_cast<char*>(&ptsRet[0]), numPoints * sizeof(mypt3d));

	dataFile.close();

	return ptsRet;
}

void MyFile::writeToFile(vecPoints & pts)
{
	std::ios_base::sync_with_stdio(false);
	
	std::ofstream dataFile;
	dataFile.open(m_filename, std::ios::out | std::ios::binary);
	
	dataFile.write(reinterpret_cast<char*> (&pts[0]), pts.size() * sizeof(mypt3d));

	dataFile.close();
}

