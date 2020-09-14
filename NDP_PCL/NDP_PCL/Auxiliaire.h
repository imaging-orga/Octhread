#ifndef AUXILIAIRE_HPP
#define AUXILIAIRE_HPP

#define BOOST_USE_WINDOWS_H
#include <tuple>
#include <iostream>
#include <chrono>
#include <thread>

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/predef/other/endian.h>
#include <pcl/visualization/point_cloud_color_handlers.h>
#include <pcl/visualization/keyboard_event.h>
#include <pcl/outofcore/visualization/camera.h>
#include <pcl/filters/voxel_grid.h>

#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>

void PosCamera(pcl::visualization::Camera camera);
float CoordMaxX(pcl::PointCloud<pcl::PointXYZRGB>::Ptr liste);
float CoordMaxY(pcl::PointCloud<pcl::PointXYZRGB>::Ptr liste);
float CoordMaxZ(pcl::PointCloud<pcl::PointXYZRGB>::Ptr liste);
std::tuple<float, float, float> DistanceAxe(pcl::PointCloud<pcl::PointXYZRGB>::Ptr face);
pcl::PointCloud<pcl::PointXYZRGB>::Ptr downSample_(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointcloud, pcl::IndicesPtr indices, float x, float y, float z);



#endif
