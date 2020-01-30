#include "LASfile.h"
#include "liblas/liblas.hpp"
#include <fstream>
LASfile::LASfile(std::string _name, long _MAXPOINTSNUMPERNODE) : OpenableFile(_name, _MAXPOINTSNUMPERNODE), maxPointsPerNode(_MAXPOINTSNUMPERNODE) {
	boost::filesystem::path p(_name);
	CreateDirectory(p.stem().string().c_str(), NULL);

	BoundingBox bb = getBoundingBox();
	p_oct = Octree(p.stem().string().c_str(), bb, 0, _MAXPOINTSNUMPERNODE);
}

BoundingBox LASfile::getBoundingBox() {
	std::ifstream ifs;
	ifs.open(p_name, std::ios::in | std::ios::binary);

	liblas::ReaderFactory f;
	liblas::Reader reader = f.CreateWithStream(ifs);

	liblas::Header const& header = reader.GetHeader();
	
	ifs.close();
	return BoundingBox(pt3d(header.GetMinX(), header.GetMinY(), header.GetMinZ()), pt3d(header.GetMaxX(), header.GetMaxY(), header.GetMaxZ()));
}

inline float distance_non_rootsquared(mypt3d& pt1, mypt3d& pt2) {
	float dX = pt2.x - pt1.x;
	float dY = pt2.y - pt1.y;
	float dZ = pt2.z - pt1.z;

	return dX * dX + dY * dY + dZ * dZ;
}

//DistMax => filtre de distance
void LASfile::read(float distMax) {
	std::ifstream ifs;
	ifs.open(p_name, std::ios::in | std::ios::binary);

	liblas::ReaderFactory f;
	liblas::Reader reader = f.CreateWithStream(ifs);

	liblas::Header const& header = reader.GetHeader();

	long long int numPoints = header.GetPointRecordsCount();

	long int maxPerChuncks = std::min(numPoints, (long long int)maxPointsPerNode);
	long int cpt = 0;
	std::vector<mypt3d> pts;
	mypt3d zero(0, 0, 0, 0, 0, 0, 0);
	while (reader.ReadNextPoint()) {
		liblas::Point const& p = reader.GetPoint();
		mypt3d pt = mypt3d(p.GetX(), p.GetY(), p.GetZ(), p.GetIntensity(), p.GetColor().GetRed(), p.GetColor().GetGreen(), p.GetColor().GetBlue());
		if (distMax != 0.0 && distance_non_rootsquared(pt, zero) < distMax * distMax) {
			pts.push_back(pt);
			cpt++;
			if (cpt >= maxPerChuncks) {
				p_oct.addPoint(pts);
				cpt = 0;
				pts.clear();
				pts.resize(0);
			}
		}
	}
	if (pts.size() != 0) {
		p_oct.addPoint(pts);
	}

	p_oct.save();
	p_oct.clean();
	ifs.close();

}