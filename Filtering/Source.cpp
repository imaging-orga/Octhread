#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../Octhread/base.hpp"
#include "../ProceedOcthread/e57File.h"
#include "Filtering.h"
#include "FiltersParam.hpp"

int main(int argc, char* argv[]) {
	//Lire un fichier et le transformé en 1 fichier Octree (Proceed Octree)


	PARAMS::filter_params par(false, 0, 0, true, 0.01, true, 20, 3);
	std::string name = "../PointClouds/Bains_TS.e57";
	OpenableFile* file = new e57File(name, 1024 * 1024 * 64);
	file->read(0);
	std::cout << "Fin De la création de l'arbre" << std::endl;
	//Créer un Filtering (Filtering)

	Filtering filter("Bains_TS", "Bains_TS-f.e57");
	//filter.filterParticularFile("01");
	//Qui va filtrer puis sauvegarder le fichier (FinishOcthread)
	filter.filter(par);
	filter.finish();

}