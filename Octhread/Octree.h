#pragma once
#include "Node.h"


class Octree
{
public:
	Octree();
	Octree(std::string filename_, BoundingBox bb_, int depth_ = 0, long int maxPointsPerLeaf = 1024 * 1024 * 16);
	~Octree();

	//save informations about octree
	void save();
	//Get the node "name"
	Node* getNode(std::string name);

	//add point cloud vector in point cloud
	void addPoint(std::vector<mypt3d>& pts);

	//get a node with a specific name (just in case)
private:
	Node m_root;
	BoundingBox m_bb;
	std::string m_name;
	int m_depth;
	long int maxPointsPerLeaf;
};

