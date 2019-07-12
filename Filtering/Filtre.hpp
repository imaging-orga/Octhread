#pragma once
#include "../Octhread/base.hpp"

namespace FILTRE {
	//downSample utilis� par la version avec Octree
	pcl::PointCloud<mypt3d>::Ptr downSample_(pcl::PointCloud<mypt3d>::Ptr pointcloud, pcl::IndicesPtr indices, float x, float y, float z);
	/*
	Pour que PCL puisse downsample, il faut que dx*dy*dz < ::numeric_limit(int32_t)::max() qui vaut 2147483647  (C)
	donc si on consid�re des cubes : x^3 = C =>
	x = ~1290;  donc il faut une taille maximale de (1290 * x) = C2; Pour �tre sur on va mettre (C2 - C2/10).
	En tout cas pour un cube. Si on change x,y ou z en ok
	*/
	//A utiliser scan par scan, ou sur un groupement de scans, ou sur l'unifi�
	void downSample(pcl::PointCloud<mypt3d>::Ptr pointcloud, float x, float y, float z, bool Octree = false);

	//Statistical Remove Outliers (Voir sur PCL pour plus d'informations)
	//A utiliser scan par scan, ou sur un groupement de scans
	void statisticalRemoveOutliers(pcl::PointCloud<mypt3d>::Ptr pointcloud, int meanK, float devMulThresh);

	/*!
	 * \brief Enlever les points qui sont en dessous ou au dessus d'un seuil de distance.
	 * 
	 */
	void distanceFilter(pcl::PointCloud<mypt3d>::Ptr, float minDist, float maxDist, float Ox, float Oy, float Oz);

	//Modifie l'intensit� 
	//Value doit �tre entre 0 et 1
	void correctionGamma(pcl::PointCloud<mypt3d>::Ptr, float nMin, float nMax);
}