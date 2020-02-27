#pragma once
#include "SavableFile.h"
#include "laswriter.hpp"
class SaveLAS : public SavableFile
{
public:
	SaveLAS(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb);
	~SaveLAS();

	SaveLAS(const SaveLAS&) = delete;
	int write(std::vector<mypt3d>& pts) override;
	int writeHeader() override;
	int writeFooter() override;

private:

	//std::ofstream m_stream;
	long long m_numEffective;
	LASheader lasheader;
	LASwriter* laswriter;

	int offset_x = 0, offset_y = 0, offset_z = 0;
};

