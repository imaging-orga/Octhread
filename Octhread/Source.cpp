#include "MyFile.h"
#include <iostream>
#include <chrono>
#include "base.hpp"
#include "Octree.h"
#include <random>




void add(MyFile mf, std::vector<mypt3d>& pts, int &numpts) {
	mf.writeToFile(pts);
	numpts += (long)pts.size();
}
int main(int argc, char* argv[]) {

	std::random_device dev;
	std::mt19937  gen{ dev() };

	std::uniform_real_distribution<float> dist{ 0, 100 };
	std::vector<mypt3d> pts;
	std::vector<mypt3d> pts2;
	std::vector<mypt3d> pts3;
	int num_points = 1024 * 1024 * 128;



	for (int i = 0; i < num_points; ++i) {
		float x = dist(gen);
		float y = dist(gen);
		float z = dist(gen);

		pts.push_back(mypt3d(x,y,z,x, i ,i ,i));
	};



	std::cout << "Ici on fini" << std::endl;

	BoundingBox bb;
	bb.min = pt3d(0., 0., 0.);
	bb.max = pt3d(100., 100., 100.);

	
	Octree oct;
	oct = Octree("test", bb, 0, 1024 * 1024 * 16);

	oct.addPoint(pts);

	pts.clear();
	pts.resize(0);


	std::cout << "1 fini\n";

	for (int i = 0; i < num_points; ++i) {
		float x = dist(gen);
		float y = dist(gen);
		float z = dist(gen);

		pts2.push_back(mypt3d(x, y/4, z/8, x, i, i, i));
	}

	oct.addPoint(pts2);
	std::cout << "2 fini\n";

	

	//for (int i = 0; i < num_points; ++i) {
	//	float x = dist(gen);
	//	float y = dist(gen);
	//	float z = dist(gen);

	//	//mypt3d pt = std::make_unique<mypt3d>(i_, i_, i_, i_, i__, i__, i__);

	//	pts3.push_back(mypt3d(x, y, z, x, i, i, i));
	//}
	//oct.addPoint(pts3);


	pts2.clear();
	pts2.resize(0);
	pts3.clear();
	pts3.resize(0);
	//std::cout << " 3 fini\n";

	oct.save();

	getchar();


	return 0;
}