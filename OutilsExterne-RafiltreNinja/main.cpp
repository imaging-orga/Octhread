#include "UniqueFileFunctions.h"
#include "SeveralFileFunctions.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "../Filtering/FiltersParam.hpp"
#include "CutIntoParts.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "FichierSansPoints.hpp"
#include "Optimizer.h"
using namespace boost;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {



	PARAMS::filter_params retParam;
	std::cout << " Je Lance la coupe " << std::endl;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Help screen")
		("nameIn,i", po::value<std::string>()->required(), "nameIn")
		("nameOut,o", po::value<std::string>(), "nameOut")
		("cut, icf", po::value<std::string>(), "cut")
		("createCutFile, ccf", po::value<bool>(), "createCutFile")
		("overlap, over", po::value<float>(), "overlap")
		("retrieveInfos, ri", po::value<bool>(), "retrieveInfos")
		("nocut, nc", po::value<std::vector<double>>()->multitoken(), "nocut")
		("fileWithoutPoints, fWP", po::value<bool>(), "fileWithoutPoints")
		("optimize, opt", po::value<bool>(), "optimize")
	;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	std::string nameIn;
	std::string nameOut;
	std::string nameCutFile;
	std::string nameDir;
	bool fileWithoutPoints = false;
	bool cut;
	float overlap = 0.0f;
	bool createCutFile = false;
	bool retrieveInfo = false;
	float sizeX, sizeY, sizeZ;
	bool optimize = false;
	if (vm.count("help"))
		std::cout << desc << "\n";
	if (vm.count("nameIn")) {
		nameIn = vm["nameIn"].as<std::string>();
	}
	if (vm.count("nameOut")) {
		nameOut = vm["nameOut"].as<std::string>();
		boost::filesystem::path p(nameOut);
		nameDir = p.stem().string().c_str();
	}
	if (vm.count("cut")) {
		nameCutFile = vm["cut"].as<std::string>();
		cut = true;
	}
	if (vm.count("createCutFile")) {
		createCutFile = true;
	}
	if (vm.count("overlap")) {
		overlap = vm["overlap"].as<float>();
	}
	if (vm.count("retrieveInfos")) {
		retrieveInfo = true;
	}
	if (vm.count("fileWithoutPoints")) {
		fileWithoutPoints = true;
	}
	if (vm.count("nocut")) {
		cut = false;
		std::vector<double> values = vm["nocut"].as<std::vector<double>>();
		sizeX = values[0];
		sizeY = values[1];
		sizeZ = values[2];
	}
	if (vm.count("optimize")) {
		optimize = true;
	}

	boost::filesystem::path p(nameIn);
	std::string extension = p.extension().string();

	if (optimize) {
		if (extension == ".e57")
			OPTIMIZE::process(nameIn, nameOut);
	}
	if (fileWithoutPoints) {
		FILEWITHOUTPOINTS::RemovePointe57(nameIn, nameOut);
	}
	if (retrieveInfo) {
		UNIQUE::retrieveInfo(nameIn, extension);
	}
	if (!fileWithoutPoints && !retrieveInfo) {
		if (cut) {
			CutIntoParts CP(nameIn, ".e57", nameCutFile, overlap);
			CP.readFile();
			CP.update();
			CP.saveFiles();
			CP.clean();
		}
		if (!cut) {
			CutIntoParts CP(nameIn, ".e57", sizeX, sizeY, sizeZ, overlap);
			CP.readFile();
			CP.update();
			CP.saveFiles();
			CP.clean();
		}
	}
	std::cout << "Fini" << std::endl;
	return EXIT_SUCCESS;

}