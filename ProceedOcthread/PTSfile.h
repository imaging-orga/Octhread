#pragma once
#include "OpenableFile.h"

class PTSfile : public OpenableFile {

public:
	PTSfile(std::string _name, long MAXPOINTSNUMPERNODE);
	~PTSfile() = default;
	/*!
	* \brief Récuperer la bounding Box du fichier
	 */
	BoundingBox getBoundingBox();
	/*!
	 * \brief Lit un fichier PTS
	 * \param distMax (au cas ou c'est un fichier non unifié)
	 */
	void read(float distMax = 0) override;
private:
	void read_(std::ifstream& file, float distMax, long num);
	long int maxPointsPerNode;
};