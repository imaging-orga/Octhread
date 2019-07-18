#pragma once
#include "SavableFile.h"
#include <string>
/*!
 * Class SaveE57
 * \brief classe qui va s'occuper de sauvegarder un fichier PTX
 */
class SavePTX : public SavableFile
{
public:
	SavePTX(std::string _filename, long int _numMax, BoundingBox& _bb);
	int write(std::vector<mypt3d>& pts);
	int writeHeader();
	int writeFooter();
	SavePTX() = delete;
	~SavePTX();

private:
	long int m_numTotal;
};

