#pragma once
#include "SavableFile.h"
#include "SaveE57.h"
#include "boost/filesystem.hpp"
#include <boost/algorithm/string.hpp>
#include "SavePTX.h"
/*!
 * Class SaveFactor
 * \brief Factory des Savables file. Récuperer l'extension du fichier, pour savoir ce qu'on va créer
 * 
 */

class SaveFactor {
public:
	/*!
	 * \brief get : Récupérer le bon format de fichier et créer le bon constructeur en fonction
	 * 
	 * \param _filename
	 * \param _numMax
	 * \param _bb
	 * \return 
	 */
	static SavableFile* get(std::string _filename, long int _numMax, BoundingBox& _bb) {
		std::string ext = boost::filesystem::extension(_filename);
		std::string lower_ext = boost::to_lower_copy(ext);
		//for (auto& c : ext)
		//	c = tolower(c);
		if (lower_ext == ".e57") {
			return new SaveE57(_filename, _numMax, _bb);
		}
		if (lower_ext == ".ptx") {
			return new SavePTX(_filename, _numMax, _bb);
		}

		return nullptr;
	}
};