#include "Auxiliaire.h"

void PosCamera(pcl::visualization::Camera camera)
{
    std::cout << "Position : |X " << camera.pos[0] << "| |Y : " << camera.pos[1] << "| |Z : " << camera.pos[2] << "|" << std::endl;
    std::cout << "View : |X " << camera.view[0] << "| |Y : " << camera.view[1] << "| |Z : " << camera.view[2] << "|" << std::endl;
    std::cout << "Focal : |X " << camera.focal[0] << "| |Y : " << camera.focal[1] << "| |Z : " << camera.focal[2] << "|" << std::endl;
}

float CoordMaxX(pcl::PointCloud<pcl::PointXYZRGB>::Ptr liste)
{
    float maxX = liste->points[0].x;
    for (int i = 1; i < liste->points.size(); ++i)
    {
        maxX = (maxX < liste->points[i].x) ? liste->points[i].x : maxX;
    }
    return maxX;
}

float CoordMaxY(pcl::PointCloud<pcl::PointXYZRGB>::Ptr liste)
{
    float maxY = liste->points[0].y;
    for (int i = 1; i < liste->points.size(); ++i)
    {
        maxY = (maxY < liste->points[i].y) ? liste->points[i].y : maxY;
    }
    return maxY;
}

float CoordMaxZ(pcl::PointCloud<pcl::PointXYZRGB>::Ptr liste)
{
    float maxZ = liste->points[0].z;
    for (int i = 1; i < liste->points.size(); ++i)
    {
        maxZ = (maxZ < liste->points[i].z) ? liste->points[i].z : maxZ;
    }
    return maxZ;
}

std::tuple<float, float, float> DistanceAxe(pcl::PointCloud<pcl::PointXYZRGB>::Ptr face)
{
    float minX = face->points[0].x, maxX = face->points[0].x;
    float minY = face->points[0].y, maxY = face->points[0].y;
    float minZ = face->points[0].z, maxZ = face->points[0].z;

    for (int i = 1; i < face->points.size(); i++)
    {
        minX = (minX > face->points[i].x) ? face->points[i].x : minX;
        maxX = (maxX < face->points[i].x) ? face->points[i].x : maxX;

        minY = (minY > face->points[i].y) ? face->points[i].y : minY;
        maxY = (maxY < face->points[i].y) ? face->points[i].y : maxY;

        minZ = (minZ > face->points[i].z) ? face->points[i].z : minZ;
        maxZ = (maxZ < face->points[i].z) ? face->points[i].z : maxZ;
    }

    float dX = maxX - minX, dY = maxY - minY, dZ = maxZ - minZ, maxD;


    if (dX > dY)
    {
        maxD = dX;
        if (dZ > maxD)
            maxD = dZ;
    }
    else
    {
        maxD = dY;
        if (dZ > maxD)
            maxD = dZ;
    }
    return std::make_tuple(abs(dX), abs(dY), abs(dZ));
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr downSample_(pcl::PointCloud<pcl::PointXYZRGB>::Ptr pointcloud, pcl::IndicesPtr indices, float x, float y, float z)
{
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr filtered_pointCloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::VoxelGrid<pcl::PointXYZRGB> sor;
    sor.setInputCloud(pointcloud);
    sor.setIndices(indices);
    sor.setLeafSize(x, y, z);
    sor.setDownsampleAllData(true);
    sor.filter(*filtered_pointCloud);

    return filtered_pointCloud;
}