#include "MyFile.h"
#include <iostream>
#include <boost\thread.hpp>
#include <chrono>
#include "base.hpp"
#include "Octree.h"


int main(int argc, char* argv[]) {

	//MyFile mf("test.bin");

	//std::vector<std::unique_ptr<mypt3d>> pts;
	//int num_points = 1024 * 1024;
	//for (int i = 0; i < num_points; ++i) {
	//	float i_ = i / 10.f;
	//	int i__ = i % 255;
	//	//std::unique_ptr<mypt3d> pt = std::make_unique<mypt3d>(i_, i_, i_, i_, i__, i__, i__);

	//	pts.push_back(std::make_unique<mypt3d>(i_, i_, i_, i_, i__, i__, i__));
	//}
	//mf.createFile();
	//
	//mf.writeToFile(pts);

	//std::vector<std::unique_ptr<mypt3d>> readpts;
	//readpts = mf.readFromFile(num_points);
	BoundingBox bb;
	bb.min = pt3d(0., 0., 0.);
	bb.max = pt3d(100., 100., 100.);
	Octree oct("test", bb, 1);
	getchar();



	return 0;
}