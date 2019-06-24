#include "Node.h"
#include <functional>




Node::Node(std::string filename_, pt3d center_, pt3d halfDimension_, int depth_, long int maxPointsPerNode_) :
	m_name(filename_), m_center(center_), m_halfDimension(halfDimension_), m_depth(depth_), maxPointsPerNode(maxPointsPerNode_), m_isLeaf(true), m_numPoints(0),
	m_file(filename_)
{
	m_file.createFile();
}

void Node::addPoint(std::vector<std::unique_ptr<mypt3d>> & pts){
	if (m_isLeaf) {
		m_file.writeToFile(pts);
		m_numPoints += (long)pts.size();

		if (m_numPoints >= maxPointsPerNode) {
			createChildren();
			populateChildren();
			m_file.emptyFile();
			m_numPoints = 0;
		}
	}
	else {
		dividePoints(pts);
	}
}

void Node::dividePoints(std::vector<std::unique_ptr<mypt3d>>& pts) {

	std::vector<std::vector<std::unique_ptr<mypt3d>>> childVecs;
	childVecs.resize(8);


	#pragma omp parallel for 
	for (int i = 0; i < pts.size(); ++i) {
		int index = 0;
		auto& pt = pts[i];
		index += (pt->x > m_center.x) ? 4 : 0;
		index += (pt->y > m_center.y) ? 2 : 0;
		index += (pt->z > m_center.z) ? 1 : 0;
		childVecs[index].push_back(std::move(pts[i]));
	}

	#pragma omp parallel for
	for (int i = 0; i < 8; ++i) {
		m_children[i].addPoint(childVecs[i]);
	}
}

void Node::createChildren()
{
	m_isLeaf = false;
	for (int i = 0; i < 8; ++i) {
		pt3d newCenter = m_center;
		newCenter.x += m_halfDimension.x * (i & 4 ? .5f : -.5f);
		newCenter.y += m_halfDimension.y * (i & 2 ? .5f : -.5f);
		newCenter.z += m_halfDimension.z * (i & 1 ? .5f : -.5f);
		std::string new_name = m_name + std::to_string(i);
		m_children.emplace_back(Node(new_name, newCenter, m_halfDimension * .5f, m_depth + 1, maxPointsPerNode));
	}
}

void Node::populateChildren()
{
	//we read the file and create a pointcloud with
	std::vector<std::unique_ptr<mypt3d>> pts = m_file.readFromFile(m_numPoints);

	dividePoints(pts);
	
}

void Node::createTree_(int endDepth)
{
	if (m_depth < endDepth) {
		createChildren();
		#pragma omp parallel for
		for (int i = 0; i < 8; ++i) {
			m_children[i].createTree_(endDepth);
		}
	}
}

Node * Node::getNode(std::string name)
{

	// découper le nom en fonction des nombres qui le compose
	// 0 => root
	// 01 => root->m_children[1]
	// 021 => root->m_children[2]->m_children[1]
	return nullptr;
}

void Node::save(std::string name)
{
	//Ouvrir le fichier "name" en écriture
	if (m_isLeaf) {
		//écrire le nom ainsi que le nombre de fils
	}
	else {
		//Ecrire que c'est pas une leaf ou 0?
	}
	//fermer le fichier
}

