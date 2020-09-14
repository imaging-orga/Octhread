#pragma once
#include <string>
#include "base.hpp"
#include <vector>

#include <fstream>
#include <boost/ptr_container/ptr_vector.hpp>

/// <summary>
/// class MyFile : Fonctions pour manipuler un fichier binaire
/// </summary>
class MyFile
{
public:
	MyFile();
	MyFile(std::string filename);
	~MyFile();


	/// <summary>
	/// Creer le fichier si il n'existe pas déjà
	/// </summary>
	void createFile();

	/// <summary>
	///  Lit les points d'un fichier et rempli un vector de points
	/// </summary>
	/// <param name="numPoints"></param>
	/// <returns>Le nombre de points</returns>
	std::vector<mypt3d> readFromFile(int numPoints);

	/// <summary>
	/// Ecrit les points dans un fichier
	/// </summary>
	/// <param name="pts">Les points à écrire</param>
	void writeToFile(std::vector<mypt3d>& pts);

	/// <summary>
	///  Mets le fichier a 0 et le supprime
	/// </summary>
	void emptyFile();

private:
	std::string m_filename;
};

