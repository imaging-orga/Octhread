#pragma once
#include <string>
#include "../Octhread/base.hpp"
/*!
 * Class SavableFile
 * \brief Classe m�re pour la sauvegarde de fichiers.
 * \redefinition n�cessaire :
 *	-write(pts)
 *	-writeHeader()
 *	-writeFooter()
 */
class SavableFile
{
public:
	SavableFile(std::string _name, unsigned long long int _numMax, BoundingBox& _bb);
	/*!
	 * \brief Ecrire les points contenu dans \pts dans le fichier
	 * 
	 * \param pts : Les points � �crire dans le fichier
	 * \return Le nombre de points �crit
	 */
	virtual int write(std::vector<mypt3d>& pts) = 0;

	/*!
	 * \brief Ecrire l'header du fichier pour les fichiers qui ont besoin d'un header.
	 * \return ??? Au cas o� ???
	 */
	virtual int writeHeader() = 0;

	/*!
	 * \brief Ecrire le footer du fichier pour le fermer
	 * \return ??? Au cas o� ???
	 */
	virtual int writeFooter() = 0;
	~SavableFile() = default;

protected:
	/*!
	 * Le nom du fichier
	 */
	std::string p_name;

	/*!
	 * Le nombre max de point qu'il y aura dans le fichier
	 */
	unsigned long long int p_numMax;

	/*!
	 * La bounding box global du nuage 
	 */
	BoundingBox p_bb;
};



