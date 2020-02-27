#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "OpenableFile.h"
#include "e57File.h"
#include "PTSfile.h"
#include "OpenFactor.hpp"
int findNumOfFile(std::string _name) {
	std::string f = "essai//.OcSave";
	std::ifstream file(f, std::ios::in);
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

int main(int argc, char* argv[]) {

	

	std::string name2 = "BigNuage.e57";


	OpenableFile* file = OpenFactor::get(name2, 1024 * 1024 * 16);
	file->read(0.);

	std::string name = "01";
	std::ifstream dataFile;
	std::vector<mypt3d> ptsRet;


	dataFile.open("essai/01", std::ios::in | std::ios::binary);
	int numPoints = findNumOfFile(name);
	if (numPoints != -1) {
		ptsRet.resize(numPoints);

		dataFile.read(reinterpret_cast<char*>(&ptsRet[0]), numPoints * sizeof(mypt3d));
		std::ofstream fileRet(name + "-test", std::ios::out | std::ios::trunc);
		fileRet.close();
		fileRet.open(name + "-test", std::ios::out);
		for (int i = 0; i < ptsRet.size() /4; ++i) {
			fileRet << ptsRet[i].x << " " << ptsRet[i].y << " " << ptsRet[i].z << " " << ptsRet[i].intensity << " " << std::to_string(ptsRet[i].r) << " " << std::to_string(ptsRet[i].g)<< " " << ptsRet[i].b<< "\n";

		}
		fileRet.close();
	}

	delete file;
	getchar();
	return 0;
}