#pragma once
#include <string>
#include "../Octhread/Octree.h"
#include "../Octhread/base.hpp"
/*!
 * Classe OpenableFile
 * \brief classe mère pour l'ouverture des fichiers
 */

class OpenableFile
{
public:
	OpenableFile(std::string name, long MAXPOINTSPERNODE);
	/*!
	 * \brief récuperer la BoundingBox du fichier
	 */
	virtual BoundingBox getBoundingBox() = 0;
	/*!
	 * Fonction a implementer dans les classes filles. read un fichier
	 * \param distMax
	 */
	virtual void read(float distMax) = 0;


protected : 
	std::string p_name;
	Octree p_oct;
};

