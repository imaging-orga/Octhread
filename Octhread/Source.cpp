#include "SyncWrite.h"
#include <iostream>
#include <boost\thread.hpp>

void thread(void* data, int min, int max) {
	SyncWrite *writer = static_cast<SyncWrite*>(data);

	for (int i = min; i < max; ++i) {
		std::string i_str = std::to_string(i);
		writer->LockedWrite(i_str);
	}

}

int main(int argc, char* argv[]) {
	std::string name = "test.txt";
	SyncWrite writ(name);


	boost::thread t1(thread, &writ, 0, 10);
	boost::thread t2(thread, &writ, 10, 20);
	boost::thread t3(thread, &writ, 20, 30);
	t1.join();
	t2.join();

	writ.close();

	//#pragma omp parallel for
	//for (int i = 0; i < 100; ++i) {/*
	//	std::string i_string = std::to_string(i);
	//	writer.LockedWrite(i_string);*/
	//	std::cout << i << std::endl;
	//}

	return 0;
}