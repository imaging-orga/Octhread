#pragma once
#include "OpenableFile.h"

class PTXfile : public OpenableFile {
public:
	PTXfile(std::string _name, long MAXPOINTSNUMPERNODE);
	~PTXfile() = default;
	/*!
	* \brief Récuperer la bounding Box du fichier
	 */
	BoundingBox getBoundingBox();
	/*!
	 * \brief Lit un fichier PTX
	 * \param distMax (au cas ou c'est un fichier non unifié)
	 */
	void read(float distMax = 0) override;
	void read_(float distMax, std::ifstream& stream, long cols, long rows);
};