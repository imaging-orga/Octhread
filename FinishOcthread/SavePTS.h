#pragma once
#include "SavableFile.h"
#include <memory>
class SavePTS : public SavableFile
{
public:
	SavePTS(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb);
	~SavePTS();

	SavePTS(const SavePTS&) = delete;
	SavePTS& operator= (const SavePTS&) = delete;
	int write(std::vector<mypt3d>& pts) override;
	int writeHeader() override;
	int writeFooter() override;

private:
	std::ofstream m_stream;
	unsigned long long int m_numEffective;
};

