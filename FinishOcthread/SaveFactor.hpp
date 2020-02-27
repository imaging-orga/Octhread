#pragma once
#include "SavableFile.h"
#include "SaveE57.h"
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>
#include "SavePTS.h"
#include "SaveLAS.h"
/*!
 * Class SaveFactor
 * \brief Factory des Savables file. R�cuperer l'extension du fichier, pour savoir ce qu'on va cr�er
 * 
 */

class SaveFactor {
public:
	/*!
	 * \brief get : R�cup�rer le bon format de fichier et cr�er le bon constructeur en fonction
	 * 
	 * \param _filename
	 * \param _numMax
	 * \param _bb
	 * \return 
	 */
	static SavableFile* get(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb) {
		std::string ext = boost::filesystem::extension(_filename);
		std::string lower_ext = boost::to_lower_copy(ext);
		//for (auto& c : ext)
		//	c = tolower(c);
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

