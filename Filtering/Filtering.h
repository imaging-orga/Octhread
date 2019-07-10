#pragma once
#include "Filtre.hpp"
#include "../FinishOcthread/SaveFactor.hpp"
class Filtering
{
public:
	Filtering(std::string _foldername, std::string _savename);

	void convert(std::vector<mypt3d>& pts);
	std::vector<mypt3d> convertBack();
	void filterParticularFile(std::string _filename);

	void filter();
	~Filtering();

	void addPoints();
	void finish();
private:
	std::string m_folderName;
	std::string m_saveName;
	SavableFile* save;
	pcl::PointCloud<mypt3d>::Ptr m_actualPTS;
};

