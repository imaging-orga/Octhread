#ifndef __POINT_HPP
#define __POINT_HPP

#define PCL_NO_PRECOMPILE
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <cstdint>
#include <array>
#include <cstring>
#include <cassert>

#include "pcl/impl/instantiate.hpp"
#include <pcl/filters/passthrough.h>
#include "pcl/filters/filter.h"
#include "pcl/impl/point_types.hpp"
#include "pcl/impl/pcl_base.hpp"
#include "pcl/impl/cloud_iterator.hpp"
#include "pcl/kdtree/impl/kdtree_flann.hpp"
#include "pcl/common/impl/accumulators.hpp"

#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/point_cloud_geometry_handlers.h>
#include <pcl/filters/voxel_grid_covariance.h>
#include <pcl/filters/impl/voxel_grid.hpp>
#include <pcl/filters/impl/voxel_grid_covariance.hpp>
#include <pcl/filters/impl/statistical_outlier_removal.hpp>
#include <pcl/search/impl/search.hpp>
#include <pcl/search/impl/organized.hpp>
#include <pcl/search/impl/kdtree.hpp>
#include <pcl/filters/impl/extract_indices.hpp>
#include <pcl/filters/impl/filter.hpp>
#include <pcl/octree/impl/octree_pointcloud.hpp>
#include <pcl/filters/impl/conditional_removal.hpp>
#include <pcl/filters/impl/shadowpoints.hpp>
#include <pcl/visualization/impl/pcl_visualizer.hpp>
#include <pcl/visualization/impl/point_cloud_geometry_handlers.hpp>
/*!
 * \brief Point personnalisé, qui contient l'info de la couleur et de l'intensité, en plus de la position :)
 * 
 */
struct _MyPoint {
	PCL_ADD_POINT4D;
	PCL_ADD_RGB;
	PCL_ADD_INTENSITY;

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} /*EIGEN_ALIGN16*/;

struct EIGEN_ALIGN16 mypt3d : _MyPoint {
	inline mypt3d() {
		x = y = z = 0.0f;
		intensity = 0.0f;
		r = g = b = 0;
		//rgb = 0;
		//rgba = 0;
	}

	inline mypt3d(float _x, float _y, float _z, float _intensity, int _r, int _g, int _b) {
		x = _x;
		y = _y;
		z = _z;
		intensity = _intensity;
		r = _r;
		g = _g;
		b = _b;
		/*rgb = ((int)_r) << 16 | ((int)_g) << 8 | ((int)_b);
		rgba = 255 << 24 | ((int)_r) << 16 | ((int)_g) << 8 | ((int)_b);*/

	}

	bool isZero(double EPS) {
		return (abs(x) < EPS && abs(y) < EPS && abs(z) < EPS);
	}

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

POINT_CLOUD_REGISTER_POINT_STRUCT
(mypt3d,
(float, x, x)
(float, y, y)
(float, z, z)
(float, intensity, intensity)
(uint32_t, rgba, rgba)
)

/*!
 * \brief contient juste les X,Y,Z
 */
class pt3d {
public:
	double x, y, z;

	pt3d(double x_  = 0.f, double y_ = 0.f, double z_ = 0.f)
		: x(x_), y(y_), z(z_) 
	{}


	pt3d operator+(pt3d& pt) {
		pt3d ret;
		ret.x = x + pt.x;
		ret.y = y + pt.y;
		ret.z = z + pt.z;
		return ret;
	}

	pt3d operator-(pt3d& pt) {
		pt3d ret;
		ret.x = x - pt.x;
		ret.y = y - pt.y;
		ret.z = z - pt.z;
		return ret;
	}

	pt3d operator/(double k) {
		pt3d ret;
		ret.x = x / k;
		ret.y = y / k;
		ret.z = z / k;
		return ret;
	}

	pt3d operator*(double k) {
		pt3d ret;
		ret.x = x * k;
		ret.y = y * k;
		ret.z = z * k;
		return ret;
	}

};

/*!
 * Class BoundingBox
 * \brief BoundingBox d'un nuage de points
 */
class BoundingBox {
public:
	pt3d min, max;

	BoundingBox(pt3d min_ = pt3d(0.f, 0.f, 0.f), pt3d max_ = pt3d(0.f, 0.f, 0.f)) 
		: min(min_), max(max_) 
	{}

	pt3d getCenter() {
		return (max + min) / 2.0;
	}

	pt3d getHalfDimension() {
		return (max - min) / 2.0;
	}
};

#endif //__POINT_HPP