#pragma once
#include "Filtre.hpp"
#include "../FinishOcthread/SaveFactor.hpp"
#include "FiltersParam.hpp"


namespace F {
	/// <summary>
	/// Une fonction "externe" qui va être utilisée en test pour filtrer des vecteurs plutôt que des fichier
	/// Utilisé pour le Multi Scan
	/// </summary>
	/// <param name="pts"></param>
	/// <param name="params"></param>
	void filterParticularVector(std::vector<mypt3d>& pts, PARAMS::filter_params & params);
}

 
/// <summary>
/// Class Filtering
/// La classe qui va faire le lien entres tous les projets, et qui va actuellement permettre de filtrer un nuage de points en format Octhread
/// </summary>
class Filtering
{
public:
	Filtering(std::string _foldername, std::string _savename);

	~Filtering() = default;

	/// <summary>
	/// Filtre un fichier particulier de l'octhread avec des paramètres prédéfinis(debug)
	/// </summary>
	/// <param name="_filename"></param>
	void filterParticularFile(std::string _filename);

	/// <summary>
	/// Filtre un fichier particulier de l'octhread avec les paramètres de \params
	/// </summary>
	/// <param name="_filename"></param>
	/// <param name="params"> Les paramètres à utiliser pour filtrer</param>
	void filterParticularFile(std::string _filename, PARAMS::filter_params& params);

	/// <summary>
	/// Filtre tous les fichiers de l'Octhread un par un, avec les paramètres de \filterParticularFile
	/// </summary>
	void filter();

	/// <summary>
	/// Filtre tous les fichiers de l'Octhread un par un, avec les paramètres de \params 
	/// </summary>
	/// <param name="params">Les paramètres à utiliser pour filtrer</param>
	void filter(PARAMS::filter_params& params);

	/// <summary>
	/// Enleve tous les points de m_actualPts qui sont a 0,0,0. Ils ont été néttoyés mais pas enlevé
	/// </summary>
	void clean();
	
	/// <summary>
	/// Sauvegarde les poits de m_actualPTS dans \save
	/// </summary>
	void addPoints();

	/// <summary>
	/// Fini la sauvegarde de Save. A Appeler une fois que tous les fichiers ont été filtrés
	/// </summary>
	void finish();

private:

	/// <summary>
	/// Transforme pts en m_actualPTS
	/// </summary>
	/// <param name="pts"></param>
	void convert(std::vector<mypt3d>& pts);
	
	/// <summary>
	/// Transforme m_actualPTS en un vector de points
	/// </summary>
	/// <returns>m_actualPTS transformé en vector de points</returns>
	std::vector<mypt3d> convertBack();

	/// <summary>
	/// Le nom du dossier dans lequel est sauvegardé l'Octhread
	/// </summary>
	std::string m_folderName;

	/// <summary>
	/// Le nom sous lequel il faudra sauvegarder le fichier unifié
	/// </summary>
	std::string m_saveName;

	/// <summary>
	/// Class pour sauvegarder un fichier
	/// </summary>
	SavableFile* save;

	/// <summary>
	/// Le nuage de point au format pcl
	/// </summary>
	pcl::PointCloud<mypt3d>::Ptr m_actualPTS;
};

