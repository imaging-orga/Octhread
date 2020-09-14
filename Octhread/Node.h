#pragma once
#include "base.hpp"
#include "MyFile.h"
/// <summary>
/// Class Node: Un node de l'octree 
/// </summary>
class Node
{
public:
	Node();
	Node(std::string filename_, pt3d center_, pt3d halfDimension_, int actualDepth, long int maxPointsPerNode_ );
	~Node() = default;



	void Destroy();

	/// <summary>
	/// Add a vector of points in the current node
	/// </summary>
	/// <param name="pts"></param>
	void addPoint(std::vector<mypt3d>& pts );

	/// <summary>
	/// Create vector for children using pts
	/// </summary>
	/// <param name="pts"></param>
	void dividePoints(std::vector<mypt3d>& pts );

	/// <summary>
	/// Create the 8 childrens
	/// </summary>
	void createChildren();

	/// <summary>
	/// Populate the children with points
	/// </summary>
	void populateChildren();

	/// <summary>
	/// creation of tree with a given depth
	/// </summary>
	/// <param name="endDepth"></param>
	void createTree_(int endDepth);
	
	/// <summary>
	/// Search for the node named "name"
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	Node* getNode(std::string name);

	/// <summary>
	/// Save in ".OcSave" file the current node
	/// </summary>
	/// <param name="dirname"></param>
	/// <param name="filename"></param>
	void save(std::string dirname, std::string filename);

	/// <summary>
	/// Clean everything about a node
	/// </summary>
	void clean();

	/// <summary>
	/// Get a bounding box from a Node
	/// </summary>
	/// <returns></returns>
	BoundingBox getBB();

	/// <summary>
	/// Récupère les points d'un node
	/// </summary>
	/// <returns></returns>
	std::vector<mypt3d> getPts();
private:
	/// <summary>
	/// Le nombre de points du node
	/// </summary>
	long int m_numPoints;

	/// <summary>
	/// Si c'est une feuille ou non
	/// </summary>
	bool m_isLeaf;

	/// <summary>
	/// Le nom de la node
	/// </summary>
	std::string m_name;

	/// <summary>
	/// L'étage du node dans l'arbre
	/// </summary>
	int m_depth;

	/// <summary>
	/// fichier binaire
	/// </summary>
	MyFile m_file;

	/// <summary>
	/// Informations sur les dimension de la node
	/// </summary>
	pt3d m_center, m_halfDimension;

	/// <summary>
	/// Nombre maximal de point pour cette node
	/// </summary>
	long int maxPointsPerNode;

	/// <summary>
	/// les enfants de ce node
	/// </summary>
	/// <remarks> NULL si il n'y en a pas </remarks>
	std::vector<Node> m_children;

};

