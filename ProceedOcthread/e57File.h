#pragma once
#include "OpenableFile.h"

#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"

enum E57_TYPE { NON_UNIFIED = 0, UNIFIED };
class e57File : public OpenableFile
{
public:
	e57File(std::string _name, long MAXPOINTSNUMPERNODE);
	~e57File() = default;

	/*!
	 * \brief Récuperer la bounding Box du fichier
	 */
	BoundingBox getBoundingBox();
	/*!
	 * \brief Lit un fichier e57
	 * \param distMax (au cas ou c'est un fichier non unifié)
	 */
	void read(float distMax = 0) override;

private :
	E57_TYPE e_type;
	/*!
	 * \brief Lit un fichier Unifié 
	 */
	void read_Unified();
	/*!
	 * \brief Lit un fichier Non Unifié
	 * \param distMax pour le filtre de distance (min = 0, ça parait peu probable que le ce soit autre chose)
	 */
	void read_NonUnified(float distMax);


};

