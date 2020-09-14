#include "Filtering.h"
#include "../ProceedOcthread/Proceed.h"

void F::filterParticularVector(std::vector<mypt3d>& pts, PARAMS::filter_params & params) {
	pcl::PointCloud<mypt3d>::Ptr cloud(new pcl::PointCloud<mypt3d>);
	cloud->points.assign(pts.begin(), pts.end());
	cloud->is_dense = true;
	cloud->width = pts.size();
	cloud->height = 1;

	if (pts.size() > 1024) {
		if (params.do_downSample) {
			float size = params.downSample_size;
			FILTRE::downSample(cloud, size, size, size, true);
		}

		if (params.do_removeOutliers && cloud->points.size() > 1024) {
			float devMultThresh = params.removeOutliers_devMultThresh;
			int meanK = params.removeOutliers_meanK;
			float distance = params.removeOutliers_distance;
			if (distance > 1.0) {
				FILTRE::statisticalRemoveOutliers(cloud, meanK, devMultThresh, distance);
			}
			else {
				FILTRE::statisticalRemoveOutliers(cloud, meanK, devMultThresh);
			}
		}

		if (params.do_correctionGamma) {
			FILTRE::correctionGamma(cloud, 0.2, 0.8); //Nombres magiques, mais c'est ce qui est le mieux de ce qu'on a vu pour le moment
		}

		cloud->points.erase(
			std::remove_if(cloud->points.begin(), cloud->points.end(),
				[](const mypt3d& pt) {return pt.x == 0.0 && pt.y == 0.0 && pt.z == 0.0; }),
			cloud->points.end()
		);
	}
	pts.resize(0);
	pts.shrink_to_fit();

	pts.assign(cloud->points.begin(), cloud->points.end());
}

Filtering::Filtering(std::string _foldername, std::string _savename) : m_folderName(_foldername), m_saveName(_savename), m_actualPTS(new pcl::PointCloud<mypt3d>)
{
	BoundingBox bb = FileOct::findBBFromFile("0", _foldername); //"0" is the root so we take global boundingbox
	unsigned long long int maxPoints = FileOct::getMaxPoints(_foldername);
	save = SaveFactor::get(_savename, maxPoints, bb);
	
}

void Filtering::convert(std::vector<mypt3d>& pts) {
	m_actualPTS->points.assign(pts.begin(), pts.end());
	m_actualPTS->is_dense = true;
	m_actualPTS->width = (uint32_t)pts.size();
	m_actualPTS->height = 1;
}


std::vector<mypt3d> Filtering::convertBack() {
	std::vector<mypt3d> pts;
	pts.assign(m_actualPTS->points.begin(), m_actualPTS->points.end());
	return pts;
}

void Filtering::filterParticularFile(std::string _filename) {
	std::vector<mypt3d> pts = FileOct::getPtsFromFile(m_folderName, _filename);
	convert(pts);
	FILTRE::downSample(m_actualPTS, 0.003, 0.003, 0.003, true);
	FILTRE::statisticalRemoveOutliers(m_actualPTS, 20, 1);
	addPoints();
}



void Filtering::filterParticularFile(std::string _filename, PARAMS::filter_params & params){
	std::vector<mypt3d> pts = FileOct::getPtsFromFile(m_folderName, _filename);
	
	convert(pts);

	if (pts.size() > 1024) {
		if (params.do_downSample) {
			float size = params.downSample_size;
			FILTRE::downSample(m_actualPTS, size, size, size, true);
		}

		if (params.do_removeOutliers  && m_actualPTS->points.size() > 1024) {
			float devMultThresh = params.removeOutliers_devMultThresh;
			int meanK = params.removeOutliers_meanK;
			FILTRE::statisticalRemoveOutliers(m_actualPTS, meanK, devMultThresh);
		}

		if (params.do_correctionGamma) {
			FILTRE::correctionGamma(m_actualPTS, 0.2, 0.8); //Nombres magiques, mais c'est ce qui est le mieux de ce qu'on a vu pour le moment
		}

		clean();
	}
	addPoints();
}

void Filtering::addPoints() {
	std::vector<mypt3d> ptsToWrite = convertBack();
	save->write(ptsToWrite);
}
void Filtering::finish() {
	save->writeFooter();
}
void Filtering::filter()
{
	std::vector<std::string> fileNon0files = FileOct::nameOfNon0files(m_folderName);
	for (auto& file : fileNon0files) {
		filterParticularFile(file);
	}
}

void Filtering::clean() {
	m_actualPTS->points.erase(
		std::remove_if(m_actualPTS->points.begin(), m_actualPTS->points.end(),
			[](const mypt3d& pt) {return pt.x == 0.0 && pt.y == 0.0 && pt.z == 0.0; }),
		m_actualPTS->points.end()
	);
}

void Filtering::filter(PARAMS::filter_params & params)
{
	std::vector<std::string> fileNon0files = FileOct::nameOfNon0files(m_folderName);
	int num = fileNon0files.size() - 1;
	for (auto& file : fileNon0files) {
		filterParticularFile(file, params);
		std::cout << "Fichier " << file << " fini... " << num-- << " restant\n"  ;
	}
}

