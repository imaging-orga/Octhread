#pragma once
#include <string>
#include "base.hpp"
#include <vector>

#include <fstream>
#include <boost/ptr_container/ptr_vector.hpp>
/*!
 * Class MyFile
 *	\brief Fonctions pour manipuler un fichier binaire
 */
class MyFile
{
public:
	MyFile();
	MyFile(std::string filename);
	~MyFile();

	/*!
	 * \brief Creer le fichier si il n'existe pas encore
	 */
	void createFile();
	
	/*!
	 * \brief Lit les points d'un fichier et rempli un vector de points
	 * \param numPoints
	 * \return 
	 */
	std::vector<mypt3d> readFromFile(int numPoints);

	/*!
	 * \brief Ecrit les points dans un fichier
	 * \param pts Les points à écrire
	 */
	void writeToFile(std::vector<mypt3d>& pts);

	/*!
	 * \brief Mets le fichier a 0 et le supprime
	 */
	void emptyFile();

private:
	std::string m_filename;
};

