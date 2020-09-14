#pragma once
#include "SavableFile.h"
#include "SaveE57.h"
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>
#include "SavePTS.h"
#include "SaveLAS.h"

/// <summary>
///  Factory des Savables file. Récuperer l'extension du fichier, pour savoir ce qu'on va créer
/// </summary>
class SaveFactor {
public:
	/// <summary>
	/// Récupérer le bon format de fichier et créer le bon constructeur en fonction
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="_numMax"></param>
	/// <param name="_bb"></param>
	/// <returns></returns>
	static SavableFile* get(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb) {
		std::string ext = boost::filesystem::extension(_filename);
		std::string lower_ext = boost::to_lower_copy(ext);
		if (lower_ext == ".e57") {
			return new SaveE57(_filename, _numMax, _bb);
		}
		if (lower_ext == ".pts") {
			return new SavePTS(_filename, _numMax, _bb);
		}
		if (lower_ext == ".las" || lower_ext == ".laz") {
			return new SaveLAS(_filename, _numMax, _bb);
		}
		else
			std::cout << "Je ne comprends le fichier de sortie\n";

		return nullptr;
	}
};

