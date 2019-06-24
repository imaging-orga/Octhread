#include "MyFile.h"
#include <iostream>
#include <boost\thread.hpp>
#include <chrono>
#include "base.hpp"
#include "Octree.h"
#include <random>

int main(int argc, char* argv[]) {

	//MyFile mf("test.bin");

	std::random_device dev;
	std::mt19937  gen{ dev() };

	std::uniform_real_distribution<float> dist{ 0, 100 };
	std::vector<std::unique_ptr<mypt3d>> pts;
	int num_points = 1000;
	for (int i = 0; i < num_points; ++i) {
		float x = dist(gen);
		float y = dist(gen);
		float z = dist(gen);

		//std::unique_ptr<mypt3d> pt = std::make_unique<mypt3d>(i_, i_, i_, i_, i__, i__, i__);

		pts.push_back(std::make_unique<mypt3d>(x, y, z, x, i, i, i));
	}
	//mf.createFile();
	//
	//mf.writeToFile(pts);

	BoundingBox bb;
	bb.min = pt3d(0., 0., 0.);
	bb.max = pt3d(100., 100., 100.);

	//todo, comprendre pourquoi ça réagit comme ça pourquoi le fichier 1 est-il encore écrit
	Octree oct("test", bb, 0, 1000);


	oct.addPoint(pts);


	MyFile mf("test\\f00");
	std::vector<std::unique_ptr<mypt3d>> readpts;
	readpts = mf.readFromFile(num_points);
	getchar();

	return 0;
}