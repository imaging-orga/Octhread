#pragma once
#include "opencv2/core.hpp"
#include <vector>
#include "Part.h"
//Créer les parties en fonctions de la taille et des vecteur X,Y,Z
//on load le fichier qu'on veut découper (déjà filtré)
//On lit les points et on les mets dans les bons fichiers (recreer une arboresecnde de fichiers)
//Puis, on créer les fichiers a partir de l'arboresence. 
// Nom fichiers : [X][Y][Z].[format]



/// <summary>
/// CutIntoParts : Classe qui permet de couper un fichier de points en plusieurs petits fichiers en suivant une trame
/// </summary>
class CutIntoParts
{
public:
	CutIntoParts() = delete;
	~CutIntoParts();

	CutIntoParts(std::string _file, std::string _retExt, float _xSize, float _ySize, float _zSize, float _overlap, cv::Vec3f _xAxis = cv::Vec3f(1, 0, 0), cv::Vec3f _yAxis = cv::Vec3f(0, 1, 0), cv::Vec3f _zAxis = cv::Vec3f(0, 0, 1));

	CutIntoParts(std::string _filename, std::string _retExt, std::string _cutFile, float _overlap);
	
	/// <summary>
	/// Recuperer la Boundingbox du nuage de points
	/// </summary>
	/// <returns></returns>
	BoundingBox getBB();


	/// <summary>
	/// Selon X, Y  et Z, récupère la bonne parts dans le vecteur (unidirectionnel)
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <returns></returns>
	Part& getPart(unsigned x, unsigned y, unsigned z);

	/// <summary>
	/// Lire le fichier associé a cette classe
	/// </summary>
	void readFile();

	/// <summary>
	/// Ecrire les parts dans le fichier Save dans le but de les récuperer ensuite
	/// </summary>
	void update();

	/// <summary>
	/// Sauvegarder les fichiers en fonction de leurs nom
	/// </summary>
	void saveFiles();


	/// <summary>
	/// Rajotuer un point
	/// </summary>
	/// <param name="pt"></param>
	void addOnePoint(mypt3d & pt);

	/// <summary>
	/// Rajouter un point si le cut File est fournie
	/// </summary>
	/// <param name="pt"></param>
	void addOnePointCutFile(mypt3d & pt);

	/// <summary>
	/// Ajouter un vecteur de points
	/// </summary>
	/// <param name="pts"></param>
	void addVector(std::vector<mypt3d>& pts);
	
	/// <summary>
	/// Ecrire les points dans les fichiers binaires
	/// </summary>
	void writeInFiles();

	/// <summary>
	/// Nettoie la classe
	/// </summary>
	void clean();
	
	/// <summary>
	/// Le vecteur de parts
	/// </summary>
	std::vector<Part> parts;
private:


	std::string nameCutter;

	/// <summary>
	/// Chercher la boudingBox d'un e57
	/// </summary>
	/// <returns></returns>
	BoundingBox getBBe57();

	/// <summary>
	/// Chercher la boundingbox d'un LAS ou d'un LAZ
	/// </summary>
	/// <returns></returns>
	BoundingBox getBBLasLaz();

	/// <summary>
	/// Chercher la boundingbox d'un PTS
	/// </summary>
	/// <returns></returns>
	BoundingBox getBBPts();

	/// <summary>
	/// Dans le cas d'un e57, si la bb n'est pas spécifiée il faut parcourir les points du fichier pour récuperer la boundingBox
	/// </summary>
	/// <returns></returns>
	std::pair<pt3d, pt3d> read_NonUnifiedForBoundingBox();

	/// <summary>
	/// Lire un fichier e57
	/// </summary>
	void readE57();

	/// <summary>
	/// Lire un fichier PTS
	/// </summary>
	void readPTS();

	/// <summary>
	/// Lire un fichier LAS ou LAZ
	/// </summary>
	void readLasLaz();

	/// <summary>
	/// Appliquer le cutfile donné lors de la construction de cette classe
	/// </summary>
	void applyCutFile(std::string _filename, std::string _cutName);
	


	/// <summary>
	/// Nom du fichier a ouvrir
	/// </summary>
	std::string m_name;

	/// <summary>
	/// Axes en X, Y et Z
	/// </summary>
	cv::Vec3f m_xAxis, m_yAxis, m_zAxis;

	/// <summary>
	/// Tailles en X, Y et Z
	/// </summary>
	float m_xSize, m_ySize, m_zSize;

	/// <summary>
	/// Nombre de parts en X, Y et Z
	/// </summary>
	unsigned m_xNum, m_yNum, m_zNum;

	/// <summary>
	/// Extension de fichier de sauvegarde
	/// </summary>
	std::string m_retExt;

	/// <summary>
	/// Offset si on doit décaler le nuage de point qui n'est pas a (0,0,0);
	/// </summary>
	pt3d offset;

	/// <summary>
	/// surface de recouvrement entre les différents fichiers
	/// </summary>
	float m_overlap;

	/// <summary>
	/// Si un cutfile est fournie 
	/// </summary>
	bool cutFileIncluded;

	/// <summary>
	/// La liste des tailles en X, Y et Z
	/// </summary>
	std::vector<float> fileSizeX, fileSizeY, fileSizeZ;

	/// <summary>
	/// La taille max en X, Y et Z
	/// </summary>
	float m_maxX = 0, m_maxY = 0, m_maxZ = 0;

	/// <summary>
	/// Nom du dossier dans lequel on va sauvegarder les fichier binaire
	/// </summary>
	std::string tempNameFolder;

	/// <summary>
	/// Nom du dossier dans lequel on va sauvegarder les petits fichiers 
	/// </summary>
	std::string cuttedNameFolder;
};

