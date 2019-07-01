#pragma once
#include "../Octhread/base.hpp"

namespace FILTRE {
	//downSample utilisé par la version avec Octree
	pcl::PointCloud<mypt3d>::Ptr downSample_(pcl::PointCloud<mypt3d>::Ptr pointcloud, pcl::IndicesPtr indices, float x, float y, float z);
	/*
	Pour que PCL puisse downsample, il faut que dx*dy*dz < ::numeric_limit(int32_t)::max() qui vaut 2147483647  (C)
	donc si on considère des cubes : x^3 = C =>
	x = ~1290;  donc il faut une taille maximale de (1290 * x) = C2; Pour être sur on va mettre (C2 - C2/10).
	En tout cas pour un cube. Si on change x,y ou z en ok
	*/
	//A utiliser scan par scan, ou sur un groupement de scans, ou sur l'unifié
	void downSample(pcl::PointCloud<mypt3d>::Ptr pointcloud, float x, float y, float z, bool Octree = false);

	//Statistical Remove Outliers (Voir sur PCL pour plus d'informations)
	//A utiliser scan par scan, ou sur un groupement de scans
	void statisticalRemoveOutliers(pcl::PointCloud<mypt3d>::Ptr pointcloud, float meanK, float devMulThresh);

	//Estimation des normales
	//a utiliser scan par scan, ou sur un groupement de scans
	void EstimateNormals(pcl::PointCloud<mypt3d>::Ptr cloud_in, pcl::PointCloud<pcl::Normal>::Ptr cloud_normals, float radius);


	//Enlever le bruit constitué par la continuité d'un laser sur une surface réflchissante (apres un tuyau par exemple).
	//A utiliser scan par scan, ou sur un groupement de scans
	//Il va calculer le produit scalaire de la normal en un point, avec la direction du laser, et si le résultat est égale a 0 (les vecteurs sont perpendiculaires) c'est que le point est sur une arete, ou la continuité d'une arete et donc on le dégage.
	void removeOrthogonalNoise(pcl::PointCloud<mypt3d>::Ptr cloud_in, float radiusNormals, float tolerance);

	//Enlever les points qui sont en dessous ou au dessus d'un seuil de distance.
	void distanceFilter(pcl::PointCloud<mypt3d>::Ptr, float minDist, float maxDist, float Ox, float Oy, float Oz);

	//Modifie l'intensité 
	//Value doit être entre 0 et 1
	void correctionGamma(pcl::PointCloud<mypt3d>::Ptr, float nMin, float nMax);
}