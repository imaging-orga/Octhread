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
	std::string filename_ = m_name + "\\OcSave";
	//Todo :
	//Creer un systeme pour sauvegarder tous les nodes, pour connaitre leur nombre de points et tout et tout.
	m_root.save(filename_);
}

Node * Octree::getNode(std::string name)
{
	return m_root.getNode(name);
}

void Octree::addPoint(std::vector<std::unique_ptr<mypt3d>>& pts)
{
	m_root.addPoint(pts);
}
