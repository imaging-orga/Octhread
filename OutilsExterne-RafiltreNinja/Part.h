#pragma once
#include <string>
#include "../Octhread/base.hpp"
#include "../Octhread/MyFile.h"


/// <summary>
/// Class Part : Une des division du fichier
/// </summary>/
class Part
{
public:
	Part();
	~Part();

	Part(std::string name, unsigned _x, unsigned _y, unsigned _z, BoundingBox _bb, float _sizeX, float _sizeY, float _sizeZ, pt3d& offset);


	/// <summary>
	/// Récupere les points du fichier binaire et écris le fichier final
	/// </summary>
	/// <param name="base"></param>
	/// <param name="baseTemp"></param>
	/// <param name="ext"></param>
	void savePart(std::string base, std::string baseTemp, std::string ext);

	/// <summary>
	/// MEt a jour le fichier save
	/// </summary>
	/// <param name="base"></param>
	void update(std::string base);

	bool isIn(mypt3d & pt);
	bool isIn(mypt3d& pt, float overlap);
	/// <summary>
	/// Ajouter un point a cette partie
	/// </summary>
	/// <param name="pt"></param>
	void addPoint(mypt3d& pt);

	/// <summary>
	/// Ecrire le vecteur dans le fichicer binaire
	/// </summary>
	void writeInFile();

	int size();

	/// <summary>
/// Nombre de points dans le fichier
/// </summary>
	unsigned long m_numPts;

private:
	///<summary>
	/// Renvoie un nom a partir de X,Y,Z
	/// </summary>
	std::string getName(std::string base);

	void saveFile2(std::string name, std::string ext, std::vector<mypt3d>& pts);

	/// <summary>
	/// Ecrire le fichier en fonction de l'extension de sortie
	/// </summary>
	/// <param name="name"></param>
	/// <param name="ext"></param>
	/// <param name="pts"></param>
	void saveFile(std::string name, std::string ext, std::vector<mypt3d>& pts);

	/// <summary>
	/// Recupere les points du fichier binaiare
	/// </summary>
	/// <returns></returns>
	std::vector<mypt3d> getPtsFromFile();

	/// <summary>
	/// BoundingBox de la partie
	/// </summary>
	BoundingBox m_bb;



	/// <summary>
	/// Placement en X, Y et Z
	/// </summary>
	unsigned m_x, m_y, m_z;

	/// <summary>
	/// Nombre de part en X, Y et Z
	/// </summary>
	float m_size_X, m_size_Y, m_size_Z;

	/// <summary>
	/// Les points de la partie
	/// </summary>
	std::vector<mypt3d> m_pts;

	/// <summary>
	/// Fichier binaire qui sera utilisé 
	/// </summary>
	MyFile m_file;

	pt3d offset;
};

