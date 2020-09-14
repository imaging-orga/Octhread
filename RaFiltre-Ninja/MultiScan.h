#pragma once
#include "../Filtering/Filtering.h"



enum FILE_OUT_TYPE {
	UNIQUE_FILE = 0,
	MULTIPLE_FILE = 1
};

class MultiScan {
public :
	MultiScan(std::string _nameIn, std::string _nameOut);
	

	/// <summary>
	/// Lancer le processus de filtrage de multifichier
	/// </summary>
	/// <returns></returns>
	bool process(PARAMS::filter_params& par, FILE_OUT_TYPE type_out);

	/// <summary>
	/// Filtrer un fichier multi-e57 vers un fichier multi-e57
	/// </summary>
	/// <param name="par"></param>
	/// <param name="type_out"></param>
	/// <returns></returns>
	bool processE57(PARAMS::filter_params & par, FILE_OUT_TYPE type_out);

	/// <summary>
	/// Filtrer un fichier multi-e57 vers un fichier multi-PTX
	/// </summary>
	/// <param name="par"></param>
	/// <param name="type_out"></param>
	/// <returns></returns>
	bool processPTX(PARAMS::filter_params & par, FILE_OUT_TYPE type_out);

	/// <summary>
	/// Creer un fichier e57 qui ne possede que 100 points (juste pour recuperer la position des scans; vu que celle ci est perdue quand on filtre)/
	/// </summary>
	/// <returns></returns>
	bool RemovePointe57();


private :
	//name Input/Output
	std::string m_nameIn, m_nameOut;
	
};
