#pragma once

#include "OpenableFile.h"
#include "e57File.h"
#include "PTSfile.h"
#include "LASfile.h"
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>
/// <summary>
/// Factor des Openables File. Récuperer l'extension du fichier, pour savoir ce qu'on va read
/// </summary>
class OpenFactor {
public:
	static OpenableFile* get(std::string _filename, long MAXNUMPERNODE) {
		std::string ext = boost::filesystem::extension(_filename);
		std::string lower_ext = boost::to_lower_copy(ext);

		if (lower_ext == ".e57") {
			return new e57File(_filename, MAXNUMPERNODE);
		}
		else if (lower_ext == ".pts") {
			return new PTSfile(_filename, MAXNUMPERNODE);
		}
		else if (lower_ext == ".las" || lower_ext == ".laz") {
			return new LASfile(_filename, MAXNUMPERNODE);
		}
		else
			std::cout << "impossible de lire l'extension du fichier";
		return nullptr;
	}

};