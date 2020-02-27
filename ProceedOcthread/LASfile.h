#pragma once
#include "OpenableFile.h"
#include "lasreader.hpp"
class LASfile : public OpenableFile {
public :
	LASfile(std::string _name, long MAXPOINTSNUMPERNODE);
	~LASfile() = default;

	BoundingBox getBoundingBox();

	void read(float distMax = 0) override;

private:
	long int maxPointsPerNode;
	LASreader* lasreader;
};