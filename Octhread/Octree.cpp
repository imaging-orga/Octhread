#include "Octree.h"





Octree::Octree(std::string filename_, BoundingBox bb_, int depth_, long int maxPointsPerLeaf_)
	: m_name(filename_), m_bb(bb_), m_depth(depth_), maxPointsPerLeaf(maxPointsPerLeaf_),
	m_root( filename_ + "\\0", bb_.getCenter(), bb_.getHalfDimension(), 0, maxPointsPerLeaf_)
{
	m_root.createTree_(depth_);
}


Octree::~Octree()
{
}

void Octree::save()
{
	std::string filename_ = m_name + "\\.OcSave";
	std::ofstream file(filename_, std::ios::out);
	file.close();
	m_root.save(filename_);
}

Node * Octree::getNode(std::string name)
{
	return m_root.getNode(name);
}

void Octree::addPoint(std::vector<mypt3d>& pts)
{
	m_root.addPoint(pts);
}
