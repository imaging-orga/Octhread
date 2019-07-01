#include "Filtre.hpp"


pcl::PointCloud<mypt3d>::Ptr FILTRE::downSample_(pcl::PointCloud<mypt3d>::Ptr pointcloud, pcl::IndicesPtr indices, float x, float y, float z) {

	pcl::PointCloud<mypt3d>::Ptr filtered_pointCloud(new pcl::PointCloud<mypt3d>);
	pcl::VoxelGrid<mypt3d> sor;
	sor.setInputCloud(pointcloud);
	sor.setIndices(indices);
	sor.setLeafSize(x, y, z);
	sor.setDownsampleAllData(true);
	sor.filter(*filtered_pointCloud);

	return filtered_pointCloud;
}

void FILTRE::downSample(pcl::PointCloud<mypt3d>::Ptr pointcloud, float x, float y, float z, bool octree) {
	if (pointcloud->points.size() > 0) {
		if (!octree) { // Suffisament petit
			pcl::VoxelGrid<mypt3d> sor;
			sor.setInputCloud(pointcloud);
			sor.setLeafSize(x, y, z);
			sor.setDownsampleAllData(true);
			sor.filter(*pointcloud);
		}
		else { //Si on doit diviser
			// dans PCL, il faut que la taille d'une box soit tel que x*y*z (taille de la box) < 2^32 / 2 (signed)
			double sizeOfLeaf = ((1290 * x) * 9.) / 10.; //90%
			pcl::octree::OctreePointCloud<mypt3d> octree(sizeOfLeaf);
			octree.setInputCloud(pointcloud);
			octree.defineBoundingBox();
			octree.addPointsFromInputCloud();

			int cpt = 0;
			pcl::PointCloud<mypt3d>::Ptr filtered_cloud(new pcl::PointCloud<mypt3d>);
			pcl::octree::OctreePointCloud<mypt3d>::LeafNodeIterator a_it;
			for (a_it = octree.leaf_depth_begin(); a_it != octree.leaf_depth_end(); ++a_it) {
				if (a_it.isLeafNode()) {
					std::vector<int> indexVector;
					a_it.getLeafContainer().getPointIndices(indexVector);
					boost::shared_ptr<std::vector<int>> indicesptr(new std::vector<int>(indexVector));

					*filtered_cloud += *downSample_(pointcloud, indicesptr, x, y, z);
					cpt++;

				}
			}
			*pointcloud = *filtered_cloud;


		}
	}
}

void FILTRE::statisticalRemoveOutliers(pcl::PointCloud<mypt3d>::Ptr pointcloud, float meanK, float devMulThresh) {
	if (pointcloud->points.size() > 0) {
		pcl::StatisticalOutlierRemoval<mypt3d> sor;
		sor.setInputCloud(pointcloud);
		if (meanK > 0.0f)
			sor.setMeanK(meanK);
		if (devMulThresh > 0.0f)
			sor.setStddevMulThresh(devMulThresh);

		sor.filter(*pointcloud);
	}
}

void FILTRE::EstimateNormals(pcl::PointCloud<mypt3d>::Ptr cloud_in, pcl::PointCloud<pcl::Normal>::Ptr cloud_normals, float radius) {
	//On va utiliser openMP pour aller plus vite quand même :)
	pcl::NormalEstimationOMP<mypt3d, pcl::Normal> ne(0);
	ne.setInputCloud(cloud_in);

	pcl::search::KdTree<mypt3d>::Ptr tree(new pcl::search::KdTree<mypt3d>());
	ne.setSearchMethod(tree);

	ne.setRadiusSearch(radius);
	ne.compute(*cloud_normals);
	std::cout << "Normal estimated" << std::endl;
}

#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/console/parse.h>

void FILTRE::removeOrthogonalNoise(pcl::PointCloud<mypt3d>::Ptr cloud_in, float radiusSearch, float tolerance) {
	if (cloud_in->points.size() > 0) {
		pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
		EstimateNormals(cloud_in, cloud_normals, radiusSearch);

		pcl::ShadowPoints<mypt3d, pcl::Normal> shadowfilters(true);
		//sets the source point cloud
		shadowfilters.setInputCloud(cloud_in);
		shadowfilters.setNormals(cloud_normals);
		double shadowThreshold = tolerance;
		shadowfilters.setThreshold(shadowThreshold);
		shadowfilters.filter(*cloud_in);
	}
}
//On utilise la distance au carré, pour gagner un peu de vitesse sur le filtre... C'est dérisoire mais c'est déjà ça. (ça ne modifie pas l'ordre de grandeur)
//puisque le sqrt est une opération "longue"
float squaredEuclidianDist(float x, float y, float z, mypt3d& pt) {
	return (pt.x - x) * (pt.x - x) + (pt.y - y) * (pt.y - y) + (pt.z - z) * (pt.z - z);
}

float squaredEuclidianDistCenter(mypt3d& pt) {
	return (pt.x * pt.x) + (pt.y * pt.y) + (pt.z * pt.z);
}


void FILTRE::distanceFilter(pcl::PointCloud<mypt3d>::Ptr cloud_in, float minDist, float maxDist, float Ox, float Oy, float Oz) {
	if (cloud_in->points.size() > 0) {
		pcl::ExtractIndices<mypt3d> extract;
		int cpt = 0;
		pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
		for (int i = 0; i < cloud_in->points.size(); ++i) {
			mypt3d& pt = cloud_in->points[i];
			float dist = squaredEuclidianDist(Ox, Oy, Oz, pt);
			float squaredMinDist = minDist * minDist;
			float squaredMaxDist = maxDist * maxDist;

			if (dist <= squaredMinDist || dist >= squaredMaxDist) {
				inliers->indices.push_back(i);
			}
		}
		extract.setInputCloud(cloud_in);
		extract.setIndices(inliers);
		extract.setNegative(true);
		extract.filter(*cloud_in);
	}

}

void FILTRE::correctionGamma(pcl::PointCloud<mypt3d>::Ptr pts, float nMin, float nMax)
{
	float oMin = 0;
	float oMax = 1;


	for (int i = 0; i < pts->points.size(); ++i) {
		//pts->points[i].intensity = sqrt(pts->points[i].intensity);
		float oldValue = pts->points[i].intensity;
		pts->points[i].intensity = (((oldValue - oMin) * (nMax - nMin)) / (oMax - oMin)) + nMin;

	}
}