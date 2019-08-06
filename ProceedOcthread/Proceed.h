#pragma once
#include <string>
#include "../Octhread/Octree.h"
#include "../Octhread/base.hpp"

/*!
 * \brief Fonctions sur les fichiers
 * 
 */
namespace FileOct{
	/*!
	 * \brief Trouver le nombre de points entier du fichier (somme de toutes les lignes dans le .OcSave)
	 * \param _fileptsName le nom du dossier
	 * \param _name le nom du fichier
	 */
	long int findNumPointsFromFile(std::string _fileptsName, std::string _name);
	/*!
	 * \brief Récupère la BoundingBox du root. Pour avoir la bounding box globale
	 * \param _fileptsName le nom du dossier
	 * \param _name le nom du fichier
	 */
	BoundingBox findBBFromFile(std::string _fileptsName, std::string _name);

	/*!
	 * \brief Récupérer les points d'un fichier
	 * \param _fileptsName le nom du dossier
	 * \param _name le nom du fichier
	 */
	std::vector<mypt3d> getPtsFromFile(std::string _fileptsName, std::string _name);

	/*!
	 * \brief Récupère le nombre de points total du fichier (l'addition de tous les petits fichiers)
	 * \param _fileptsName le nom du dossier
	 */
	unsigned long long int getMaxPoints(std::string _fileptsName);

	/*!
	 * \brief Récupèrela liste des fichiers qui ont un nombre de point supérieur à 0
	 * \params _fileptsName le nom du dossier
	 */
	std::vector<std::string> nameOfNon0files(std::string  _fileptsName);
}