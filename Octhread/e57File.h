#pragma once
#include "OpenableFile.h"


enum E57_TYPE { NON_UNIFIED = 0, UNIFIED };
class e57File : public OpenableFile
{
public:
	e57File(std::string _name, long MAXPOINTSNUMPERNODE);
	~e57File() = default;

	BoundingBox getBoundingBox();
	void read();

private :
	E57_TYPE e_type;

	void read_Unified();
	
	void read_NonUnified();
};

