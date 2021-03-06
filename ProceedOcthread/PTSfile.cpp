#include "PTSfile.h"

PTSfile::PTSfile(std::string _name, long MAXPOINTSNUMPERNODE) : OpenableFile(_name, MAXPOINTSNUMPERNODE), maxPointsPerNode(MAXPOINTSNUMPERNODE)
{
	boost::filesystem::path p(_name);
	CreateDirectory(p.stem().string().c_str(), NULL);

	BoundingBox bb = getBoundingBox();
	p_oct = Octree(p.stem().string().c_str(), bb, 0, MAXPOINTSNUMPERNODE);
}



float getLeicaIntens(int value) {
	//[-2048, 2048] -> [0,1]
	//[+2048] [/4096]

	return ((float)value + 2048.) / 4096.;
}

BoundingBox PTSfile::getBoundingBox()
{
	//long int bufferSize = 1024 * 1024 * 128;
	std::ifstream file(p_name, std::ios::in);
	float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max(), minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min(), maxY = std::numeric_limits<float>::min(), maxZ = std::numeric_limits<float>::min();
	std::string line;
	std::getline(file, line);
	long unsigned maxLine = std::atol(line.c_str());

	for (int i = 0; i < maxLine; ++i) {
		std::getline(file, line);
		std::istringstream ss(line);
		float tmpX, tmpY, tmpZ;
		ss >> tmpX >> tmpY >> tmpZ;
		if (tmpX < minX)
			minX = tmpX;
		if (tmpY < minY)
			minY = tmpY;
		if (tmpZ < minZ)
			minZ = tmpZ;

		if (tmpX > maxX)
			maxX = tmpX;
		if (tmpY > maxY)
			maxY = tmpY;
		if (tmpZ > maxZ)
			maxZ = tmpZ;
	}

	file.close();
	return BoundingBox(pt3d(minX, minY, minZ), pt3d(maxX, maxY, maxZ));
	//lire le fichier en entier
}

void PTSfile::read(float distMax)
{
	std::ifstream file(p_name, std::ios::in);
	while (true) {
		std::string line;
		std::getline(file, line);
		std::istringstream ss(line);
		long numPts;
		ss >> numPts;
		read_(file, distMax, numPts);

		if (file.eof()) break;
	}
	p_oct.save();
	p_oct.clean();
	file.close();
}

inline float distance_non_rootsquared(mypt3d& pt1, mypt3d& pt2) {
	float dX = pt2.x - pt1.x;
	float dY = pt2.y - pt1.y;
	float dZ = pt2.z - pt1.z;

	return dX * dX + dY * dY + dZ * dZ;
}
void PTSfile::read_(std::ifstream& file,float distMax, long number) {
	
	//A refaire!
	
	std::string line;

	//Savoir si on a 3, 4 ou 7 valeurs au PTS
	std::getline(file, line);
	std::stringstream ss(line);
	std::string token;
	int i = 0;
	while (std::getline(ss, token, ' ')) {
		i++;
	}
	bool color = false, intens = false;
	switch (i) {
	case 4:
		intens = true;
		break;
	case 7:
		color = true;
		intens = true;
		break;
	}
	
	std::vector<mypt3d> pts;
	mypt3d zero(0, 0, 0, 0, 0, 0, 0);
	long int size = std::min(number, maxPointsPerNode);
	//X, Y, Z
	if (!intens && !color) { 
		mypt3d pt;
		std::istringstream ssb(line);
		ssb >> pt.x >> pt.y >> pt.z;
		if (distMax != 0.0 && distance_non_rootsquared(pt, zero) < distMax * distMax) {
			pt.intensity = 0.5;
			pt.r = pt.b = pt.r = 128;
			pts.push_back(pt);
		}
		for (int j = 1; j < size; ++j) {
			std::getline(file, line);
			ssb = std::istringstream(line);
			ssb >> pt.x >> pt.y >> pt.z;
			if (distMax != 0.0 && distance_non_rootsquared(pt, zero) < distMax * distMax) {
				pt.intensity = 0.5;
				pt.r = pt.g = pt.b = 128;
				pts.push_back(pt);
			}
		}
	}
	//X,Y,Z,I
	else if (intens && !color) { 
		mypt3d pt;
		std::istringstream ssb(line);
		ssb >> pt.x >> pt.y >> pt.z >> pt.intensity;
		if (distMax != 0.0 && distance_non_rootsquared(pt, zero) < distMax * distMax) {
			pt.r = pt.b = pt.r = 128;
			pts.push_back(pt);
		}
		for (int j = 1; j < size; ++j) {
			std::getline(file, line);
			ssb = std::istringstream(line);
			ssb >> pt.x >> pt.y >> pt.z;
			if (distMax != 0.0 && distance_non_rootsquared(pt, zero) < distMax * distMax) {
				ssb >> pt.intensity;
				pt.intensity = getLeicaIntens(pt.intensity);
				pt.r = pt.g = pt.b = pt.intensity * 255;
				pts.push_back(pt);
			}
		}
	}
	//X,Y,Z,I,R,G,B
	else { 
		mypt3d pt;
		std::istringstream ssb(line);
		float intens;
		ssb >> pt.x >> pt.y >> pt.z >> intens >> pt.r >> pt.g >> pt.b;

		pt.intensity = getLeicaIntens(intens);
		if (distMax == 0.0 || distance_non_rootsquared(pt, zero) < distMax * distMax) {
			pts.push_back(pt);
		}
		for (int j = 1; j < size; ++j) {
			std::getline(file, line);
			ssb = std::istringstream(line);
			ssb >> pt.x >> pt.y >> pt.z;
			if (distMax == 0.0 || distance_non_rootsquared(pt, zero) < distMax * distMax) {
				intens;
				ssb >> intens;
				pt.intensity = getLeicaIntens(intens);
				unsigned r, g, b;
				ssb >> r >> g >> b;
				pt.r = (int8_t)r; pt.g = (int8_t)g; pt.b = (int8_t)b;
				pts.push_back(pt);
			}
		}
	}
	p_oct.addPoint(pts);
}