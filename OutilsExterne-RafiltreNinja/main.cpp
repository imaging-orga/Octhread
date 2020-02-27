#include "UniqueFileFunctions.h"
#include "SeveralFileFunctions.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "../Filtering/FiltersParam.hpp"

using namespace boost;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {
	boost::filesystem::path p(argv[1]);
	//std::string nameInfo = p.stem().string() + ".info"; /*fichier.info*/
	std::string ext = p.extension().string();

	if (ext == ".e57") {
		return UNIQUE::retrieveInfoe57(argv[1]);
	}
	else if (ext == ".las" || ext == ".laz") {
		return UNIQUE::retrieveInfolasz(argv[1]);
	}
	else if (ext == ".pts") {
		return UNIQUE::retrieveInfopts(argv[1]);
	}

	return EXIT_SUCCESS;

}