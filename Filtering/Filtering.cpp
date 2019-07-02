#include "Filtering.h"



Filtering::Filtering(std::string _filename) : saveFile(_filename)
{
}


void Filtering::filter()
{
	int num = 0;
	//for (every file in folder) {
	//	std::vector<mypt3d> pts;
	//	pts = applyfilters();
	// num += saveFile.write();
	//}
	saveFile.finishWriting();

}

Filtering::~Filtering()
{
}
