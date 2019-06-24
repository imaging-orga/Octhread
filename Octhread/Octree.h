#pragma once
#include "Node.h"


class Octree
{
public:
	Octree(std::string filename_, BoundingBox bb_, int depth_ = 0, long int maxPointsPerLeaf = 1024 * 1024 * 16);
	~Octree();

	//save informations about octree
	void save();
	//Get the node that is named after "name"
	Node* getNode(std::string name);

	//add point cloud vector in point cloud
	void addPoint(std::vector<std::unique_ptr<mypt3d>>& pts);
private:
	Node m_root;
	BoundingBox m_bb;
	std::string m_name;
	int m_depth;
	long int maxPointsPerLeaf;
};

