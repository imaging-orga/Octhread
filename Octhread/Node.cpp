#include "Node.h"
#include <functional>
#include <iostream>


Node::Node() {}

Node::Node(std::string filename_, pt3d center_, pt3d halfDimension_, int depth_, long int maxPointsPerNode_) :
	m_name(filename_), m_center(center_), m_halfDimension(halfDimension_), m_depth(depth_), maxPointsPerNode(maxPointsPerNode_), m_isLeaf(true), m_numPoints(0),
	m_file(filename_)
{
	if (m_isLeaf)
		m_file.createFile();
}



void Node::Destroy() {
	m_numPoints = 0;
	remove(m_name.c_str());
}




void Node::addPoint(std::vector<mypt3d>& pts ){

	if (m_isLeaf){
		if (m_numPoints + pts.size() >= maxPointsPerNode){
			createChildren();
			dividePoints(pts);
			if (m_numPoints > 0) {
				std::vector<mypt3d> ptsFromFile = m_file.readFromFile(m_numPoints);
				dividePoints(ptsFromFile);
			}
			Destroy();
		}
		else{
			if (pts.size() > 0) {
				m_file.writeToFile(pts);
				m_numPoints += (long)pts.size();
			}
		}
	}
	else { //non leaf
		dividePoints(pts);
	}
	
	

}

void Node::dividePoints(std::vector<mypt3d>& pts ) {

	std::vector<std::vector<mypt3d>> childVecs;
	childVecs.resize(8);


	//#pragma omp parallel for 
	for (int i = 0; i < pts.size(); ++i) {
		int index = 0;
		auto& pt = pts[i];
		index += (pt.x > m_center.x) ? 4 : 0;
		index += (pt.y > m_center.y) ? 2 : 0;
		index += (pt.z > m_center.z) ? 1 : 0;
		childVecs[index].push_back(pts[i]);
	}

	//#pragma omp parallel for
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
	std::vector<mypt3d> pts = m_file.readFromFile(m_numPoints);

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

	std::vector<char> data(name.begin(), name.end());
	if (data.size() == 1) {
		return this;
	}
	else {
		data.erase(data.begin());
		std::string new_name = std::string(data.begin(), data.end());
		return m_children[(int)(data[0]) - '0'].getNode(new_name);
	}
}

void Node::save(std::string dirname, std::string filename)
{
	
	std::string prev_name = m_name;
	std::string new_name = prev_name.erase(0, dirname.size() + 1 /*on enleves le "\" aussi*/);
	BoundingBox bb = getBB();
	if (m_isLeaf) {

		std::ofstream file(filename, std::ios::out | std::ios::app);
		
		file << new_name << " " << m_numPoints << " " 
			<< bb.min.x << " " << bb.min.y << " " << bb.min.z << " "
			<< bb.max.x << " " << bb.max.y << " " << bb.max.z << "\n";
		file.close();
	}
	else {
		std::ofstream file(filename, std::ios::out | std::ios::app);
		file << new_name << " " << 0 << " "
			<< bb.min.x << " " << bb.min.y << " " << bb.min.z << " "
			<< bb.max.x << " " << bb.max.y << " " << bb.max.z << "\n";
		file.close();
		for (auto& child : m_children) {
			child.save(dirname, filename);
		}
	}
}

void Node::clean()
{
	if (m_numPoints == 0) {
		remove(m_name.c_str());
	}
	if (!m_isLeaf) {
		for (int i = 0; i < 8; ++i) {
			m_children[i].clean();
		}
	}
}

BoundingBox Node::getBB()
{

	pt3d min, max;
	min = m_center - m_halfDimension;
	max = m_center + m_halfDimension;
	return BoundingBox(min, max);
}

std::vector<mypt3d> Node::getPts()
{
	return m_file.readFromFile(m_numPoints);
}

