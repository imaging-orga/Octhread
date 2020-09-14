#pragma once
#include <string>
#include "../Octhread/Octree.h"
#include "../Octhread/base.hpp"

/// <summary>
/// classe m�re pour l'ouverture des fichiers
/// </summary>
class OpenableFile
{
public:
	OpenableFile(std::string name, long MAXPOINTSPERNODE);
	/// <summary>
	/// r�cuperer la BoundingBox du fichier
	/// </summary>
	/// <returns></returns>
	virtual BoundingBox getBoundingBox() = 0;

	/// <summary>
	/// Fonction a implementer dans les classes filles. read un fichier
	/// </summary>
	/// <param name="distMax">Au cas ou le filtre de distance soit activ�</param>
	virtual void read(float distMax) = 0;


protected : 
	/// <summary>
	/// Nom du fichier
	/// </summary>
	std::string p_name;

	/// <summary>
	/// L'Octree associ�
	/// </summary>
	Octree p_oct;
};

