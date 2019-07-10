#include "MyFile.h"

#include <sstream>

MyFile::MyFile() {}

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

std::vector<mypt3d> MyFile::readFromFile(int numPoints)
{
	std::ifstream dataFile;
	std::vector<mypt3d> ptsRet;

	dataFile.open(m_filename, std::ios::in | std::ios::binary);
	ptsRet.resize(numPoints);

	dataFile.read(reinterpret_cast<char*>(&ptsRet[0]), numPoints * sizeof(mypt3d));
	dataFile.close();
	return ptsRet;

}


void MyFile::writeToFile(std::vector<mypt3d> & pts)
{
#if !debug
	std::ios_base::sync_with_stdio(false);
	
	std::ofstream dataFile;
	dataFile.open(m_filename, std::ios::out | std::ios::binary | std::ios::app);
	dataFile.write(reinterpret_cast<char*> (&pts[0]), pts.size() * sizeof(mypt3d));


	dataFile.close();
#else
	std::ofstream dataFile;
	dataFile.open(m_filename, std::ios::out | std::ios::app);
	for (int i = 0; i < pts.size(); ++i) {
		dataFile << pts[i].get()->x << " " << pts[i].get()->y << " " << pts[i].get()->z << " " << pts[i].get()->intens << " "  << pts[i].get()->r << " " << pts[i].get()->g << " " << pts[i].get()->b << "\n";
	}
	dataFile.close();
#endif
}

void MyFile::emptyFile()
{
	std::ofstream file(m_filename, std::ios::out | std::ios::trunc);
	file.close();
}

