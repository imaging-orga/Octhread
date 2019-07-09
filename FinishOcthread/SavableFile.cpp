#include "SavableFile.h"


SavableFile::SavableFile(std::string _name, long int _numMax, BoundingBox& _bb) : p_name(_name), p_numMax(_numMax), p_bb(_bb)
{
}


SavableFile::~SavableFile()
{
}
