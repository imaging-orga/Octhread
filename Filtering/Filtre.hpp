#pragma once
#include "../Octhread/base.hpp"

namespace FILTRE {
	/// <summary>
	/// downSample utilisé par la version avec Octree
	/// </summary>
	/// <param name="pointcloud"></param>
	/// <param name="indices"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <returns></returns>
	pcl::PointCloud<mypt3d>::Ptr downSample_(pcl::PointCloud<mypt3d>::Ptr pointcloud, pcl::IndicesPtr indices, float x, float y, float z);

	/// <summary>
	/// Applique le filtre downsample a un nuage de point
	/// </summary>	
	/// <remark>Pour que PCL puisse downsample, il faut que dx*dy*dz inferieur a ::numeric_limit(int32_t)::max() qui vaut 2147483647  (C) 
	/// donc si on considère des cubes : x ^ 3 = C = >
	/// x = ~1290;  donc il faut une taille maximale de(1290 * x) = C2; Pour être sur on va mettre(C2 - C2 / 10).
	/// </remark>
	/// <param name="pointcloud"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <param name="Octree"></param>
	void downSample(pcl::PointCloud<mypt3d>::Ptr pointcloud, float x, float y, float z, bool Octree = false);

	/// <summary>
	/// Statistical Remove Outliers (Voir sur PCL pour plus d'informations)
	/// </summary>
	/// <param name="pointcloud"></param>
	/// <param name="meanK"></param>
	/// <param name="devMulThresh"></param>
	void statisticalRemoveOutliers(pcl::PointCloud<mypt3d>::Ptr pointcloud, int meanK, float devMulThresh);


	/// <summary>
	/// Statistical Remove Outliers (Voir sur PCL pour plus d'informations). On va de plus ajouter un argument "distance" et ne pas s'occuper des points qui sont au dela de "distance".
	/// Nécessite un multiScan forcement en entrée.
	/// </summary>
	/// <param name="pointcloud"></param>
	/// <param name="meanK"></param>
	/// <param name="devMulThresh"></param>
	/// <param name="distance"></param>
	void statisticalRemoveOutliers(pcl::PointCloud<mypt3d>::Ptr pointcloud, int meanK, float devMulThresh, float distance);


	/// <summary>
	/// Statistical Remove Outliers avec distance thresh. On va pas toucher aux points qui ont une distance au centre plus grande que thresh
	/// </summary>
	/// <param name="pointcloud"></param>
	/// <param name="meanK"></param>
	/// <param name="devMulThresh"></param>
	/// <param name="distThresh"></param>
	void statisticalRemoveOutliers(pcl::PointCloud<mypt3d>::Ptr pointcloud, int meanK, float devMulThresh, float distThresh);
	/// <summary>
	/// Enlever les points qui sont en dessous ou au dessus d'un seuil de distance.
	/// </summary>
	/// <param name=""></param>
	/// <param name="minDist"></param>
	/// <param name="maxDist"></param>
	/// <param name="Ox"></param>
	/// <param name="Oy"></param>
	/// <param name="Oz"></param>
	void distanceFilter(pcl::PointCloud<mypt3d>::Ptr, float minDist, float maxDist, float Ox, float Oy, float Oz);

	/// <summary>
	/// Modifie l'intensité 
	/// </summary>
	/// <remark> l'intensité doit être comprise entre 0 et 1 </remark>
	/// <param name=""></param>
	/// <param name="nMin"></param>
	/// <param name="nMax"></param>//
	//Value doit être entre 0 et 1
	void correctionGamma(pcl::PointCloud<mypt3d>::Ptr, float nMin, float nMax);
}