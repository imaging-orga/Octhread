#pragma once
#include "SavableFile.h"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"

#include <combaseapi.h>


struct e57WriteData {
	std::vector<double> xData, yData, zData;
	std::vector<int8_t> isInvalidData;
	std::vector<double> intData;
	std::vector<uint16_t> redData, greenData, blueData;
};

class SaveE57 : public SavableFile
{
public:
	SaveE57(std::string _filename);
	int write(std::vector<mypt3d>& pts) override;
	int writeHeader() override;
	int writeFooter() override;
	~SaveE57();
/*
	e57::CompressedVectorWriter dataWriter;*/
private:
	e57::Writer eWriter;
	long int sizeChunks;
	long int pointCount = 1024* 1024 * 8 * 4;
	int scanIndex;
	e57WriteData datas;
};

