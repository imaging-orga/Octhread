#pragma once
#include <string>
#include "../Octhread/Octree.h"
#include "../Octhread/base.hpp"

/// <summary>
/// Fonctions sur les fichiers
/// </summary>
namespace FileOct{
	/// <summary>
	/// Trouver le nombre de points entier du fichier (somme de toutes les lignes dans le .OcSave)
	/// </summary>
	/// <param name="_fileptsName"></param>
	/// <param name="_name"></param>
	/// <returns></returns>
	long int findNumPointsFromFile(std::string _fileptsName, std::string _name);
	/// <summary>
	/// Récupère la BoundingBox du root. Pour avoir la bounding box globale
	/// </summary>
	/// <param name="_fileptsName"></param>
	/// <param name="_name"></param>
	/// <returns></returns>
	BoundingBox findBBFromFile(std::string _fileptsName, std::string _name);

	/// <summary>
	///  Récupérer les points d'un fichier
	/// </summary>
	/// <param name="_fileptsName"></param>
	/// <param name="_name"></param>
	/// <returns></returns>
	std::vector<mypt3d> getPtsFromFile(std::string _fileptsName, std::string _name);

	/// <summary>
	/// Récupère le nombre de points total du fichier (l'addition de tous les petits fichiers)
	/// </summary>
	/// <param name="_fileptsName"></param>
	/// <returns></returns>
	unsigned long long int getMaxPoints(std::string _fileptsName);

	/// <summary>
	/// Récupèrela liste des fichiers qui ont un nombre de point supérieur à 0
	/// </summary>
	/// <param name="_fileptsName"></param>
	/// <returns></returns>
	std::vector<std::string> nameOfNon0files(std::string  _fileptsName);
}