#include "Filtering.h"
#include "../ProceedOcthread/Proceed.h"

Filtering::Filtering(std::string _foldername, std::string _savename) : m_folderName(_foldername), m_saveName(_savename), m_actualPTS(new pcl::PointCloud<mypt3d>)
{
	BoundingBox bb = FileOct::findBBFromFile("0", _foldername); //"0" is the root so we take global boundingbox
	long int maxPoints = FileOct::getMaxPoints(_foldername);
	save = SaveFactor::get(_savename, maxPoints, bb);
	
}

void Filtering::convert(std::vector<mypt3d>& pts) {
	m_actualPTS->points.assign(pts.begin(), pts.end());
	//m_actualPTS->points.resize(pts.size());
	//for (int i = 0; i < pts.size(); ++i) {
	//	m_actualPTS->points[i] = pts[i];
	//}
	m_actualPTS->is_dense = true;
	m_actualPTS->width = pts.size();
	m_actualPTS->height = 1;
}


std::vector<mypt3d> Filtering::convertBack() {
	std::vector<mypt3d> pts;
	//pts.resize(m_actualPTS->points.size());
	//for (int i = 0; i < m_actualPTS->points.size(); ++i) {
	//	pts[i] = m_actualPTS->points[i];
	//}
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

Filtering::~Filtering()
{
}
