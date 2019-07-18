#pragma once
#include "Filtre.hpp"
#include "../FinishOcthread/SaveFactor.hpp"
#include "FiltersParam.hpp"
/*!
 * Class Filtering
 * \brief La classe qui va faire le lien entres tous les projets, 
 * et qui va actuellement permettre de filtrer un nuage de points en format Octhread
 */
class Filtering
{
public:
	Filtering(std::string _foldername, std::string _savename);

	~Filtering() = default;
	/*!
	 * \brief Filtre un fichier particulier de l'octhread avec des paramètres prédéfinis(debug)
	 * \param _filename le nom du fichier
	 */
	void filterParticularFile(std::string _filename);
	/*!
	 * \brief Filtre un fichier particulier de l'octhread avec les paramètres de \params
	 * \param _filename le nom du fichiers
	 * \param params : Les paramètres à utiliser pour filtrer
	 */
	void filterParticularFile(std::string _filename, PARAMS::filter_params& params);

	/*!
	 * \brief Filtre tous les fichiers de l'Octhread un par un, avec les paramètres de \filterParticularFile
	 */
	void filter();

	/*!
	 * \brief Filtre tous les fichiers de l'Octhread un par un, avec les paramètres de \params 
	 * 
	 * \param params : Les paramètres à utiliser pour filtrer
	 */
	void filter(PARAMS::filter_params& params);

	/*!
	 * \brief Enleve tous les points de m_actualPts qui sont a 0,0,0. Ils ont été néttoyés mais pas enlevé
	 */
	void clean();
	
	/*!
	 * \brief Sauvegarde les poits de m_actualPTS dans \save
	 */
	void addPoints();

	/*!
	 * \brief Fini la sauvegarde de Save. A Appeler une fois que tous les fichiers ont été filtrés
	 */
	void finish();
private:
	/*!
	 * \brief Transforme \pts en m_actualPTS
	 * \param pts : Un vector de points
	 */
	void convert(std::vector<mypt3d>& pts);

	/*!
	 * \brief Transforme m_actualPTS en un vector de points
	 * \return m_actualPTS transformé en vector de points
	 */
	std::vector<mypt3d> convertBack();


	/*!
	 * Le nom du dossier dans lequel est sauvegardé l'Octhread
	 */
	std::string m_folderName;
	/*!
	 * Le nom sous lequel il faudra sauvegarder le fichier unifié
	 */
	std::string m_saveName;
	/*!
	 * Class pour sauvegarder un fichier
	 */
	SavableFile* save;
	/*!
	 * Le nuage de point au format pcl (todo : Faire les changements pour ne plus avoir a copier
	 * le vector<mypt3d> en ce format à chaque fois qu'on veut travailler dessus => gain de temps? (minime peut-être?)
	 */
	pcl::PointCloud<mypt3d>::Ptr m_actualPTS;
};

