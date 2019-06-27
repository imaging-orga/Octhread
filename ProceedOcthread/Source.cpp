#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
class mypt3d {
public:
	float x, y, z;
	float intens;
	int r, g, b;

	mypt3d(float x_ = 0.f, float y_ = 0.f, float z_ = 0.f, float intens_ = 0.f, int r_ = 0, int g_ = 0, int b_ = 0)
		: x(x_), y(y_), z(z_), intens(intens_), r(r_), g(g_), b(b_)
	{}
};

int findNumOfFile(std::string _name) {
	std::string f = ".OcSave";
	std::ifstream file(f, std::ios::in);
	std::string line;
	int num = -1;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		std::string filename;
		ss >> filename;
		if (filename == _name) {
			ss >> num;
		}

	}
	return num;
}

int main(int argc, char* argv[]) {

	std::string name = "001";
	std::ifstream dataFile;
	std::vector<mypt3d> ptsRet;

	
	dataFile.open(name, std::ios::in | std::ios::binary);
	int numPoints = findNumOfFile(name);
	if (numPoints != -1) {
		ptsRet.resize(numPoints);

		dataFile.read(reinterpret_cast<char*>(&ptsRet[0]), numPoints * sizeof(mypt3d));
		std::ofstream fileRet(name + "-test", std::ios::out | std::ios::trunc);
		fileRet.close();
		fileRet.open(name + "-test", std::ios::out);
		for (int i = 0; i < ptsRet.size(); ++i) {
			fileRet << ptsRet[i].x << " " << ptsRet[i].y << " " << ptsRet[i].z << " " << ptsRet[i].intens << " " << ptsRet[i].r << " " << ptsRet[i].g << " " << ptsRet[i].b << "\n";
		}
		fileRet.close();

		
	}

	dataFile.close();
	return 0;
}