#include "Octree.h"





Octree::Octree(std::string filename_, BoundingBox bb_, int depth_, long int maxPointsPerLeaf_)
	: m_name(filename_), m_bb(bb_), m_depth(depth_), maxPointsPerLeaf(maxPointsPerLeaf_),
	m_root( filename_ + "\\f0", bb_.getCenter(), bb_.getHalfDimension(), 0, maxPointsPerLeaf)
{
	m_root.createTree_(depth_);
}


Octree::~Octree()
{
}
