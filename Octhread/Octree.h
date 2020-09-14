#pragma once
#include "Node.h"

/// <summary>
/// Class Octree : Fonctions sur l'octree
/// </summary>
class Octree
{
public:
	Octree();
	Octree(std::string filename_, BoundingBox bb_, int depth_ = 0, long int maxPointsPerLeaf = 1024 * 1024 * 16);
	~Octree();

	/// <summary>
	/// save informations about octree
	/// </summary>
	void save();

	/// <summary>
	/// Get the node "name"
	/// </summary>
	Node* getNode(std::string name);

	/// <summary>
	/// add point cloud vector in point cloud
	/// </summary>
	/// <param name="pts"></param>
	void addPoint(std::vector<mypt3d>& pts);

	/// <summary>
	/// clean the tree
	/// </summary>
	void clean();
private:
	/// <summary>
	/// Racine de l'arbre
	/// </summary>
	Node m_root;

	/// <summary>
	/// Bounding box de l'arbre
	/// </summary>
	BoundingBox m_bb;

	/// <summary>
	/// Nom de l'arbre
	/// </summary>
	std::string m_name;

	/// <summary>
	/// Profondeur de l'arbre
	/// </summary>
	int m_depth;

	/// <summary>
	/// Nombre de points max par feuilles
	/// </summary>
	long int maxPointsPerLeaf;
};

