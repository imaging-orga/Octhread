#pragma once
#include <string>
#include "../Octhread/base.hpp"
/*!
 * Class SavableFile
 * \brief 
 * \
 */

/// <summary>
/// Classe mère pour la sauvegarde de fichiers.
/// </summary>
/// <remark>
/// redefinition nécessaire :
/// -write(pts)
/// -writeHeader()
/// -writeFooter()
/// </remark>
class SavableFile
{
public:
	SavableFile(std::string _name, unsigned long long int _numMax, BoundingBox& _bb);
	 
	/// <summary>
	/// Ecrire les points contenu dans \pts dans le fichier
	/// </summary>
	/// <param name="pts">Les points à écrire dans le fichier</param>
	/// <returns>Le nombre de points écrit</returns>
	virtual int write(std::vector<mypt3d>& pts) = 0;

	/// <summary>
	/// Ecrire l'header du fichier pour les fichiers qui ont besoin d'un header.
	/// </summary>
	/// <returns>Si besoin est de retourner quelque chose (erreur?)</returns>
	virtual int writeHeader() = 0;

	/// <summary>
	/// Ecrire le footer du fichier pour le fermer
	/// </summary>
	/// <returns>Si besoin est de retourner quelque chose (erreur?)</returns>
	virtual int writeFooter() = 0;

	~SavableFile() = default;

protected:

	/// <summary>
	/// Le nom du fichier
	/// </summary>
	std::string p_name;
	
	/// <summary>
	///  Le nombre max de point qu'il y aura dans le fichier
	/// </summary>
	unsigned long long int p_numMax;

	/// <summary>
	/// La bounding box global du nuage 
	/// </summary>
	BoundingBox p_bb;
};



