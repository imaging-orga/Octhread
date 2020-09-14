#include <cstdio>
#include <iostream>
#include "../Filtering/FiltersParam.hpp"
#include "../ProceedOcthread/e57File.h"
#include "../Filtering/Filtering.h"
#include "NinjaTurtle.hpp"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "../ProceedOcthread/OpenFactor.hpp"
#include <shellapi.h>
#include "MultiScan.h"
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
		("nonUnified, nnunif", po::value<int>(), "nonUnified")
		("cut, c", po::value<std::string>(), "cut")
		("overlap, overla", po::value<float>(), "overlap")
		("nocut, nc", po::value<std::vector<double>>()->multitoken(), "nocut")

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
		if (vm["removeOutliers"].as<std::vector<double>>().size() != 3) {
			std::cerr << "Error while parsing, removeOutliers need 2 arguments [meanK, devMultThres]\n";
			return 1;
		}
		else {
			std::vector<double> values = vm["removeOutliers"].as<std::vector<double>>();
			retParam.do_removeOutliers = true;
			retParam.removeOutliers_meanK = values[0];
			retParam.removeOutliers_devMultThresh = values[1];
			retParam.removeOutliers_distance = values[2];
		}
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

	if (vm.count("nonUnified")) {
		int value = vm["nonUnified"].as<int>();
		retParam.nonUnified_option = value;
		retParam.do_nonUnified = true;
		//retParam.do_nonUnified = value;
	}

	if (vm.count("cut")) {
		std::string value = vm["cut"].as<std::string>();
		retParam.do_cut = true;
		retParam.cut_fileIn = value;
	}

	if (vm.count("overlap")) {
		float value = vm["overlap"].as<float>();
		retParam.cut_overlap = value;
	}

	if (vm.count("nocut")) {
		std::vector<double> values = vm["nocut"].as<std::vector<double>>();
		retParam.do_nocut = true;
		retParam.nocut_X = values[0];
		retParam.nocut_Y = values[1];
		retParam.nocut_Z = values[2];
	}
	return retParam;
}

VOID startup(std::string lpApplicationName, std::string arguments)
{
	HINSTANCE hRet = ShellExecute(
		HWND_DESKTOP, //Parent window
		"open",       //Operation to perform
		lpApplicationName.c_str(),       //Path to program
		arguments.c_str(),         //Parameters
		NULL,         //Default directory
		SW_SHOW);     //How to open
}

int main(int argc, char* argv[]) {

	PARAMS::filter_params par = getParameters(argc, argv);


	if (par.do_nonUnified) { // Multiple scan
		MultiScan ms(par.nameIn, par.nameOut);
		ms.process(par, (FILE_OUT_TYPE)par.nonUnified_option);
	}
	else {

		if (par.do_tree) {
			OpenableFile* file = OpenFactor::get(par.nameIn, par.tree_sizeLeaf);
			if (par.do_distance)
				file->read(par.distance_max);
			else
				file->read(0.);
			std::cout << "Fin De la création de l'arbre" << std::endl;
		}
		//Créer un Filtering (Filtering)
		
		Filtering filter(par.nameDir, par.nameOut);
		filter.filter(par);
		filter.finish();
		if (par.do_potree) { //Si on doit faire le potree
			startup("datas\\PotreeConverter.exe", par.nameOut.c_str());

		}
		
		if (!par.do_cut && !par.do_nocut) {
			drawNinja();
			getchar();
		}

	}
	
	std::cout << par.do_cut << " " << par.do_nocut << std::endl;
	if (par.do_cut) {
		std::string arguments = "--nameIn \"" + par.nameOut + "\" --cut \"" + par.cut_fileIn + "\" --overlap " + std::to_string(par.cut_overlap);
		startup("datas\\OutilsExternes.exe", arguments);

		drawNinja();
		getchar();
	}
	if (par.do_nocut) {
		std::string arguments = "--nameIn \"" + par.nameOut + "\" --nocut " + std::to_string(par.nocut_X) + " " + std::to_string(par.nocut_Y) + " " + std::to_string(par.nocut_Z) + " --overlap " + std::to_string(par.cut_overlap);
		startup("datas\\OutilsExternes.exe", arguments);

		drawNinja();
		getchar();
	}
	
	
	getchar();
	
}