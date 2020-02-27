#include "LASfile.h"
#include "laszip.hpp"
#include <fstream>
LASfile::LASfile(std::string _name, long _MAXPOINTSNUMPERNODE) : OpenableFile(_name, _MAXPOINTSNUMPERNODE), maxPointsPerNode(_MAXPOINTSNUMPERNODE) {
	boost::filesystem::path p(_name);
	CreateDirectory(p.stem().string().c_str(), NULL);

	LASreadOpener lasreadopener;
	lasreadopener.set_file_name(p_name.c_str());

	lasreader = lasreadopener.open();
	BoundingBox bb = getBoundingBox();
	std::string test = p.stem().string();
	p_oct = Octree(p.stem().string(), bb, 0, _MAXPOINTSNUMPERNODE);


}

BoundingBox LASfile::getBoundingBox() {

	double minX = lasreader->get_min_x();
	double minY = lasreader->get_min_y();
	double minZ = lasreader->get_min_z();
	double maxX = lasreader->get_max_x();
	double maxY = lasreader->get_max_y();
	double maxZ = lasreader->get_max_z();

	if (minX == 0.0 && minY == 0.0 && minZ == 0.0 && maxX == 0.0 && maxY == 0.0 && maxZ == 0.0) { // aucune donnée, on va devoir le calculé...

	//	minX = std::numeric_limits<double>::max();
	//	minY = std::numeric_limits<double>::max();
	//	minZ = std::numeric_limits<double>::max();

	//	maxX = std::numeric_limits<double>::min();
	//	maxY = std::numeric_limits<double>::min();
	//	maxZ = std::numeric_limits<double>::min();
	//	while (lasreader->read_point()) { //Long!! mais si la donnée n'est pas calculé, c'est la hess
	//		LASpoint& laspt = lasreader->point;

	//		if (laspt.get_x() < minX)
	//			minX = laspt.get_x();
	//		if (laspt.get_y() < minY)
	//			minY = laspt.get_y();
	//		if (laspt.get_z() < minZ)
	//			minZ = laspt.get_z();


	//		if (laspt.get_x() > maxX)
	//			maxX = laspt.get_x();
	//		if (laspt.get_y() > maxY)
	//			maxY = laspt.get_y();
	//		if (laspt.get_z() > maxZ)
	//			maxZ = laspt.get_z();
	//	}

		minX = -1000;
		minY = -1000;
		minZ = -1000;
		maxX = 1000;
		maxY = 1000;
		maxZ = 1000;
	}

	return BoundingBox(pt3d(minX, minY, minZ), pt3d(maxX, maxY, maxZ));
}

//Plus rapide que la version avec le sqrt
inline float distance_non_rootsquared(LASpoint& pt) {

	float dX = pt.get_x();
	float dY = pt.get_y();
	float dZ = pt.get_z();


	return dX * dX + dY * dY + dZ * dZ;
}

//DistMax => filtre de distance
void LASfile::read(float distMax) {

	LASpoint zero;
	std::vector<mypt3d> ptsVec;

	unsigned sizeBuff = 1024 * 1024 * 64;/*Magic value, buffering 67'108'864 rows*/
	ptsVec.resize(sizeBuff);
	int cpt = 0;

	while (lasreader->read_point()) {
		
		LASpoint& laspt = lasreader->point;
		if (distance_non_rootsquared(laspt) > distMax * distMax) {
			/*[0, 65'536] (U16) -> [0, 1]*/
			mypt3d pt(laspt.get_x(), laspt.get_y(), laspt.get_z(), (float)laspt.get_I() / 65535.0f, laspt.rgb[0] / 255, laspt.rgb[1] / 255, laspt.rgb[1] / 255);

			ptsVec[cpt] = pt; //faire un vector
			cpt++;
			
			if (cpt == sizeBuff - 1) {
				cpt = 0;
				p_oct.addPoint(ptsVec);
			}
		}

	}
	ptsVec.resize(cpt);
	p_oct.addPoint(ptsVec);

	p_oct.save();
	lasreader->close();
}