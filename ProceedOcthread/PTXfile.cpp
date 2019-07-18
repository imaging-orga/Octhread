#include "PTXfile.h"

PTXfile::PTXfile(std::string _name, long MAXPOINTSNUMPERNODE) : OpenableFile(_name, MAXPOINTSNUMPERNODE)
{
	boost::filesystem::path p(_name);
	CreateDirectory(p.stem().string().c_str(), NULL);

	BoundingBox bb = getBoundingBox();
	p_oct = Octree(p.stem().string().c_str(), bb, 0, MAXPOINTSNUMPERNODE);
}

BoundingBox PTXfile::getBoundingBox()
{
	//Todo
	return BoundingBox();
}

void PTXfile::read(float distMax){
	std::ifstream file(p_name, std::ios::in);
	while (true) {
		long int cols, rows;
		file >> cols >> rows;
		read_(distMax, file, cols, rows);

		if (file.eof()) break;	
	}
	//while (!file.eof())
	file.close();
}

void PTXfile::read_(float distMax, std::ifstream& file, long cols, long rows) {
	//Recuperer la matrice de trasnformation
	//Multiplier tous les points par cette matrice
	//Créer un vectuer
	//Read un scan dans le fichier
	//Todo :: A revoir
	float tmp;

	std::string line;

	for (int i = 0; i < 4; ++i) {
		std::getline(file, line);
		std::istringstream lins(line);
		lins >> tmp >> tmp >> tmp;
	}
	float r11, r12, r13,
		r21, r22, r23,
		r31, r32, r33;
	float t11, t12, t13;

	std::getline(file, line);
	std::istringstream lins(line);
	lins >> r11 >> r12 >> r13 >> tmp;
	std::getline(file, line);
	lins = std::istringstream(line);
	lins >> r21 >> r22 >> r23 >> tmp;
	std::getline(file, line);
	lins = std::istringstream(line);
	lins >> r31 >> r32 >> r33 >> tmp;
	std::getline(file, line);
	lins = std::istringstream(line);
	lins >> t11 >> t12 >> t13 >> tmp;

	std::vector<mypt3d>pts;
	int sizeBuff = 1024 * 1024 * 128;
	pts.resize(sizeBuff);
	int i;
	for (i = 0; i < cols * rows; ++i) {
		std::getline(file, line);
		std::istringstream ss(line);
		float x, y, z;
		float intens;
		int8_t r, g, b;
		ss >> x >> y >> z >> intens >> r >> g >> b;
		float new_x, new_y, new_z;
		new_x = x * r11 + y * r12 + z * r13 + t11;
		new_y = x * r21 + y * r22 + z * r23 + t12;
		new_z = x * r31 + y * r32 + z * r33 + t13;

		pts[i%sizeBuff] = mypt3d(new_x, new_y, new_z, intens, r, g, b);
		if (i%sizeBuff == sizeBuff - 1) {
			p_oct.addPoint(pts);
		}
		//les derniers points
		pts.resize(i%sizeBuff);
		p_oct.addPoint(pts);
	}
}
