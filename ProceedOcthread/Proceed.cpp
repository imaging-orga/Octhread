#include "Proceed.h"
#include <fstream>


namespace FileOct {
	long int findNumPointsFromFile(std::string _fileptsName, std::string _name) {
		std::string newName = _fileptsName + "/" + _name;
		std::string nameOcsave = _fileptsName + "/.OcSave";
		std::ifstream file(nameOcsave, std::ios::in);
		std::string line;
		int num = -1;
		while (std::getline(file, line)) {
			std::istringstream ss(line);
			std::string filename;
			ss >> filename;
			if (filename == _name) {
				ss >> num;
			}
		}
		return num;
	}

	BoundingBox findBBFromFile(std::string _fileptsName, std::string _name) {
		std::string newName = _fileptsName + "/" + _name;
		std::string nameOcsave = _fileptsName + "/.OcSave";
		std::ifstream file(nameOcsave, std::ios::in);
		std::string line;

		pt3d min, max;
		int num;
		while (std::getline(file, line)) {
			std::istringstream ss(line);
			std::string filename;
			ss >> filename;
			if (filename == _name) {
				ss >> num >> min.x >> min.y >> min.z >> max.x >> max.y >> max.z; 
			}
		}

		return BoundingBox(min, max);
	}




	std::vector<mypt3d> getPtsFromFile(std::string _fileptsName, std::string _name) {
		long int numPoints = findNumPointsFromFile(_fileptsName, _name);
		std::vector<mypt3d> ptsRet; 
		std::ifstream dataFile;

		if (numPoints != -1) {
			ptsRet.resize(numPoints);
			std::string newName = _fileptsName + "/" + _name;
			dataFile.open(newName, std::ios::in | std::ios::binary);
			dataFile.read(reinterpret_cast<char*>(&ptsRet[0]), numPoints * sizeof(mypt3d));
			dataFile.close();
		}
		return ptsRet;
	}

	unsigned long long int getMaxPoints(std::string _fileptsName)
	{
		std::string nameOcsave = _fileptsName + "/.OcSave";
		std::ifstream file(nameOcsave, std::ios::in);
		std::string line;
		unsigned long long int max = 0;
		while (std::getline(file, line)) {
			std::istringstream ss(line);
			std::string tmp;
			unsigned long long int num;
			ss >> tmp >> num;
			max += num;
		}
		return max;
	}

	std::vector<std::string> nameOfNon0files(std::string _fileptsName)
	{
		std::vector<std::string> filenames;
		std::string nameOcsave = _fileptsName + "/.OcSave";
		std::ifstream file(nameOcsave, std::ios::in);
		std::string line;
		while (std::getline(file, line)) {
			std::istringstream ss(line);
			std::string tmp;
			unsigned num;
			ss >> tmp >> num;
			if (num != 0) {
				filenames.push_back(tmp);
			}
		}

		return filenames;

	}

}