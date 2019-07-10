#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../Octhread/base.hpp"
#include "../ProceedOcthread/e57File.h"
#include "Filtering.h"
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
	//Lire un fichier et le transformé en 1 fichier Octree (Proceed Octree)

	//std::string name = "../PointClouds/essai.e57";
	//OpenableFile* file = new e57File(name, 1024 * 1024 * 16);
	//file->read(0);
	//Créer un Filtering (Filtering)

	Filtering filter("essai", "essai-f.e57");
	//filter.filterParticularFile("01");
	//Qui va filtrer puis sauvegarder le fichier (FinishOcthread)
	filter.filter();

	filter.finish();

}