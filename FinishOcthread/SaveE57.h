#pragma once
#include "SavableFile.h"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"

#include <combaseapi.h>



struct e57WriteData {
	std::vector<double> xData, yData, zData;
	std::vector<double> isInvalidData;
	std::vector<double> intData;
	std::vector<int> redData, greenData, blueData;

	e57WriteData(long int num = 0) { 
		xData.resize(num); yData.resize(num); zData.resize(num);
		isInvalidData.resize(num); intData.resize(num); 
		redData.resize(num); greenData.resize(num); blueData.resize(num); 
	}
};

class SaveE57 : public SavableFile
{
public:

	SaveE57(std::string _filename, long int _numMax, BoundingBox& _bb);
	int  writeTe(e57::CompressedVectorWriter* writer, int size_writer, std::vector<mypt3d>& pts,
		std::vector<double>& cartesianX, std::vector<double>& cartesianY, std::vector<double>& cartesianZ,
		std::vector<double>& cartesianInvalidState,
		std::vector<double>& intensity,
		std::vector<int>& red, std::vector<int>& green, std::vector<int>& blue);
	int write(std::vector<mypt3d>& pts) override;
	int writeHeader() override;
	int writeFooter() override;
	~SaveE57();
/*
	e57::CompressedVectorWriter dataWriter;*/
private:
	e57::ImageFile *imf;
	e57::Writer eWriter;
	int scanIndex;
	e57WriteData datas;
	e57::CompressedVectorWriter * writer;
	long int pointCount; //remettre pointcount
	e57::CompressedVectorNode* groups;
	long int writerChunckSize;
	long int num_max = 0;
};

