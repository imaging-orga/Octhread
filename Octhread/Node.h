#pragma once
#include "base.hpp"
#include "MyFile.h"
class Node
{
public:
	Node(std::string filename_, pt3d center_, pt3d halfDimension_, int actualDepth, long int maxPointsPerNode_ );
	~Node() = default;


	//Add a point in the current node
	void addPoint(mypt3d& pt);
	//Add a vector of points in the current node
	void addPoint(std::vector<std::unique_ptr<mypt3d>>& pts);

	void dividePoints(std::vector<std::unique_ptr<mypt3d>>& pts);

	//Create the 8 childrens
	void createChildren();
	//Populate the children with points
	void populateChildren();


	void createTree_(int endDepth);
	

private:
	long int m_numPoints;
	bool m_isLeaf;
	std::string m_name;
	int m_depth;
	MyFile m_file;
	pt3d m_center, m_halfDimension;

	long int maxPointsPerNode;

	std::vector<Node> m_children;

};

