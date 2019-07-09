#include "SaveFactor.hpp"
#include <random>


using namespace e57;
using namespace std;

std::string createGUID(){
	GUID		guid;
	CoCreateGuid((GUID*)&guid);
	OLECHAR wbuffer[64];
	StringFromGUID2(guid, &wbuffer[0], 64);
	size_t	converted = 0;
	char	strGuid[64];
	wcstombs_s(&converted, strGuid, wbuffer, 64);
	return strGuid;
}




int main(int /*argc*/, char** /*argv*/)
{
	float minX=-1000, maxX=1000;
	std::vector<mypt3d> pts1, pts2, pts3;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(minX, maxX);
	
	
	//SavableFile *file = SaveFactor::get("test.a");
	//SavableFile *file = new SaveE57("test.e57");
	//file->writeHeader();
		
	long int size = 1024 * 1024 * 32 ;
	
	for (int i = 0; i < size; ++i) {
		float i_size = i / size;
		int imodsize = i % 255;
		pts1.push_back(mypt3d(/*x*/dist(mt) /*random 0 -> 1000*/, /*y*/dist(mt)/*random 0 -> 1000*/, /*z*/0, /*intensity*/0.33, /*r*/255, /*g*/0, /*b*/0));
		pts2.push_back(mypt3d(/*x*/dist(mt) /*random 0 -> 1000*/, /*y*/dist(mt)/*random 0 -> 1000*/, /*z*/100, /*intensity*/0.66, /*r*/0, /*g*/255, /*b*/0));
		pts2.push_back(mypt3d(/*x*/dist(mt) /*random 0 -> 1000*/, /*y*/dist(mt)/*random 0 -> 1000*/, /*z*/100, /*intensity*/0.66, /*r*/0, /*g*/128, /*b*/128));
		pts3.push_back(mypt3d(/*x*/dist(mt) /*random 0 -> 1000*/, /*y*/dist(mt)/*random 0 -> 1000*/, /*z*/200, /*intensity*/0.99, /*r*/255, /*g*/255, /*b*/255));

	}

	pt3d minpt(minX, minX, minX);
	pt3d maxpt(maxX, maxX, maxX);
	BoundingBox bb(minpt, maxpt);


	SavableFile *e57file = SaveFactor::get("test1.e57", size*4, bb);
	e57file->writeHeader();
	e57file->write(pts1);
	e57file->write(pts2);
	e57file->write(pts3);
	e57file->writeFooter();

	delete e57file;

	return(0);
}

