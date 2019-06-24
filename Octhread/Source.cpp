#include "MyFile.h"
#include <iostream>
#include <boost\thread.hpp>
#include <chrono>
#include "base.hpp"
//void thread(void* data, int min, int max) {
//	SyncWrite *writer = static_cast<SyncWrite*>(data);
//
//	for (int i = min; i < max; ++i) {
//		std::string i_str = std::to_string(i);
//		writer->LockedWrite(i_str);
//	}
//
//}



//std::string trans(std::vector<mypt3d>& pts) {
//	std::string ret = "";
//	for (auto& pt : pts) {
//		ret += pt.tostring();
//	}
//	return ret;
//}

//Tech 1 : 
//Liste des tous les points 
//On écrit tout le vector d'un coup

//long long writetechnique2(std::string name, std::vector<mypt3d>& pts) {
//
//	std::ios_base::sync_with_stdio(false);
//	auto startTime = std::chrono::high_resolution_clock::now();
//
//	std::ofstream data_file;
//	data_file.open(name, std::ios::out | std::ios::binary);
//	data_file.write(reinterpret_cast<char*> (&pts[0]), pts.size() * sizeof(mypt3d));
//	data_file.close();
//	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
//}
//
//
//long long readtechnique2(std::string name, std::vector<mypt3d>& pts, int num_points) {
//	std::ifstream data_file;
//	std::vector<mypt3d> ptsRet;
//	auto startTime = std::chrono::high_resolution_clock::now();
//
//	data_file.open("test2.binary", std::ios::in | std::ios::binary);
//	ptsRet.resize(num_points);
//	data_file.read(reinterpret_cast<char*>(&ptsRet[0]), num_points * sizeof(mypt3d));
//	data_file.close();
//	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();;
//
//}


int main(int argc, char* argv[]) {
	//std::string name = "test.txt";
	//SyncWrite writ(name);
	//const clock_t begin_time = clock();

	//boost::thread t1(thread, &writ, 0, 20000);
	//boost::thread t2(thread, &writ, 20000, 40000);
	//boost::thread t3(thread, &writ, 140000, 60000);
	//t1.join();
	//t2.join();
	//t3.join();
	//writ.close();
	//std::cout << float(clock() - begin_time) / CLOCKS_PER_SEC << "\n";

	//std::string name2 = "test2.txt";
	//SyncWrite writ2(name2);
	//const clock_t begin_time2 = clock();

	//boost::thread t(thread, &writ2, 0, 60000);
	//t.join();
	//writ2.close();
	//std::cout << float(clock() - begin_time2) / CLOCKS_PER_SEC;


	////long long ret = technique1("test.binary", pts);

	////std::cout << ret << std::endl;

	//std::vector<mypt3d> pts2;
	//std::cout << writetechnique2("test2.binary", pts) << "\n";
	//std::cout << readtechnique2("test2.binary", pts2, num_points) << "\n";

	MyFile mf("test.bin");

	std::vector<std::unique_ptr<mypt3d>> pts;
	int num_points = 1024 * 1024;
	for (int i = 0; i < num_points; ++i) {
		float i_ = i / 10.f;
		int i__ = i % 255;
		//std::unique_ptr<mypt3d> pt = std::make_unique<mypt3d>(i_, i_, i_, i_, i__, i__, i__);

		pts.push_back(std::make_unique<mypt3d>(i_, i_, i_, i_, i__, i__, i__));
	}
	mf.createFile();
	
	mf.writeToFile(pts);

	std::vector<std::unique_ptr<mypt3d>> readpts;
	readpts = mf.readFromFile(num_points);

	getchar();



	return 0;
}