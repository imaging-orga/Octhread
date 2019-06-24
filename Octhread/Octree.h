#pragma once
#include "Node.h"


class Octree
{
public:
	Octree(std::string filename_, BoundingBox bb_, int depth_ = 0, long int maxPointsPerLeaf = 1024 * 1024 * 16);
	~Octree();





private:
	Node m_root;
	BoundingBox m_bb;
	std::string m_name;
	int m_depth;
	long int maxPointsPerLeaf;
};

