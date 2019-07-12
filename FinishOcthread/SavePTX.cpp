#include "SavePTX.h"






SavePTX::SavePTX(std::string _filename, long int _numMax, BoundingBox & _bb) : SavableFile(_filename, _numMax, _bb)
{
}

int SavePTX::write(std::vector<mypt3d>& pts)
{
	return 0;
}

int SavePTX::writeHeader()
{
	return 0;
}

int SavePTX::writeFooter()
{
	return 0;
}

SavePTX::~SavePTX()
{
}
