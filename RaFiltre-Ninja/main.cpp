#include <cstdio>
#include <iostream>
#include "../Filtering/FiltersParam.hpp"
#include "../ProceedOcthread/e57File.h"
#include "../Filtering/Filtering.h"
#include "NinjaTurtle.hpp"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "../ProceedOcthread/OpenFactor.hpp"
#include "Previsu.h"
#include <shellapi.h>
#include "Divide.h"
using namespace boost;
namespace po = boost::program_options;

PARAMS::filter_params getParameters(int argc, char* argv[]) {

	PARAMS::filter_params retParam;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Help screen")
		("nameIn,i", po::value<std::string>()->required(), "nameIn")
		("nameOut,o", po::value<std::string>()->required(), "nameOut")
		("distance,dist", po::value<std::vector<double>>()->multitoken(), "distance")
		("downSample,DS", po::value<double>(), "downSample")
		("removeOutliers,RO", po::value<std::vector<double>>()->multitoken(), "removeOutliers")
		("correctionGamma, CG", po::value<bool>(), "correctionGamma")
		("createTree, CT", po::value<long>(), "createTree")
		("previsu, PV", po::value<bool>(), "previsu")
		("division, div", po::value<int>(), "division")
		("Potree, pot", po::value<bool>(), "Potree")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
		std::cout << desc << "\n";
	if (vm.count("nameIn")) {
		retParam.nameIn = vm["nameIn"].as<std::string>();
		boost::filesystem::path p(retParam.nameIn);
		retParam.nameDir = p.stem().string().c_str();
	}

	if (vm.count("nameOut"))
		retParam.nameOut = vm["nameOut"].as<std::string>();

	if (vm.count("createTree")){
		long value = vm["createTree"].as<long>();
		retParam.do_tree = true;
		retParam.tree_sizeLeaf = value;
	}
	if (vm.count("distance")) {
		if (vm["distance"].as<std::vector<double>>().size() != 2) {
			std::cerr << "Error while parsing, distance need 2 arguments [min, max]\n";
			return 1;
		}
		else
		{
			std::vector<double> values = vm["distance"].as<std::vector<double>>();
			retParam.do_distance = true;
			retParam.distance_min = values[0];
			retParam.distance_max = values[1];
		}
	}
	if (vm.count("downSample")) {
		double value = vm["downSample"].as<double>();
		retParam.do_downSample = true;
		retParam.downSample_size = value;
	}
	if (vm.count("removeOutliers")) {
		if (vm["removeOutliers"].as<std::vector<double>>().size() != 2) {
			std::cerr << "Error while parsing, removeOutliers need 2 arguments [meanK, devMultThres]\n";
			return 1;
		}
		else {
			std::vector<double> values = vm["removeOutliers"].as<std::vector<double>>();
			retParam.do_removeOutliers = true;
			retParam.removeOutliers_meanK = values[0];
			retParam.removeOutliers_devMultThresh = values[1];
		}
	}

	if (vm.count("division")) {
		double value = vm["division"].as<int>();
		retParam.do_div = true;
		retParam.num_div = value;
	}
	if (vm.count("correctionGamma")) {
		bool value = vm["correctionGamma"].as<bool>();
		retParam.do_correctionGamma = value;
	}

	if (vm.count("previsu")) {
		bool value = vm["previsu"].as<bool>();
		retParam.previz = value;
	}

	if (vm.count("Potree")) {
		bool value = vm["Potree"].as<bool>();
		retParam.do_potree = value;
	}
	return retParam;
}



int main(int argc, char* argv[]) {
	//Necessite d'écrire les paramètres quand on call le programme

	//Divide essai("Pilege-PartG", 2);
	//essai.CutOcsave();
	//getchar();
	PARAMS::filter_params par = getParameters(argc, argv);

	if (par.do_tree) {
		OpenableFile* file = OpenFactor::get(par.nameIn, par.tree_sizeLeaf);
		if (par.do_distance)
			file->read(par.distance_max);
		else
			file->read(0.);
		std::cout << "Fin De la création de l'arbre" << std::endl;
	}
	//if (par.previz) {
	//	Previsu previs(par.nameIn);
	//	previs.pickWiseFile();
	//	previs.Visualize();
	//}
	
	
	
	//Créer un Filtering (Filtering)
	if (!par.do_div) {
		Filtering filter(par.nameDir, par.nameOut);
		filter.filter(par);
		filter.finish();
		if (par.do_potree) { //Si on doit faire le potree
			ShellExecuteA(
				NULL,
				"open",
				"datas\\PotreeConverter.exe",
				par.nameOut.c_str(),
				NULL,
				SW_SHOWNORMAL
			);

		}
	}
	else {//Si on doit faire une do_div
		Divide div(par.nameDir, par.num_div);
		div.CutOcsave();
		boost::filesystem::path p(par.nameOut);

		std::string pathName = p.parent_path().string();
		std::string filename = p.stem().string();
		std::string extension = p.extension().string();


		for (int i = 0; i < par.num_div; ++i) {
			std::string new_name = pathName + "\\" + filename + "_" + std::to_string(i) + extension;
			//Remplacer le Ocsave par OcSave_i;
			std::string newOcsave = par.nameDir + "\\.OcSave_" + std::to_string(i);
			std::string Ocsave = par.nameDir + "\\.OcSave";

			std::string tmp = par.nameDir + "\\tmp";

			rename(Ocsave.c_str(), tmp.c_str());
			rename(newOcsave.c_str(), Ocsave.c_str());


			//lancer le calcul ici :)
			Filtering filter(par.nameDir, new_name);
			filter.filter(par);
			filter.finish();

			rename(Ocsave.c_str(), newOcsave.c_str());
			rename(tmp.c_str(), Ocsave.c_str());


			std::cout << "Fichier " << i << " Fini :) \n\n"; 
		}
		
	}

	
	drawNinja();
	getchar();
}