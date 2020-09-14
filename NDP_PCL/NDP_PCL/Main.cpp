#define BOOST_USE_WINDOWS_H
#define TAILLE 900

#include "Auxiliaire.h"

int continuerPreTraitement = true, continuerPostTraitement = true;
float distance = 0.f, maxX, maxY, maxZ;
char derniereTouche;

pcl::PointCloud<pcl::PointXYZRGB>::Ptr face(new pcl::PointCloud<pcl::PointXYZRGB>);

pcl::PointXYZRGB center;
pcl::visualization::Camera cam;

Eigen::Matrix4f extra;
Eigen::Matrix3f intra;

std::vector<std::string>tabLine{};
pcl::PointXYZRGB p1, p2;

std::vector<float>coupureAxeXcoordZ{};
std::vector<float>coupureAxeYcoordX{};
std::vector<float>coupureAxeZcoordY{};

std::string derniereLine;

int compteurCoupure = 0;

pcl::visualization::PCLVisualizer::Ptr viewerPost(new pcl::visualization::PCLVisualizer("2e phase"));

void InitCamera(void* viewer_void)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer>*>(viewer_void);

    viewer->setCameraPosition((double)center.x, (double)center.y, (double)center.z, 0, 1, 0);
	
    extra(0, 0) = 1; extra(0, 1) = 0; extra(0, 2) = 0; extra(0, 3) = (double)center.x;
    extra(1, 0) = 0; extra(1, 1) = 1; extra(1, 2) = 0; extra(1, 3) = (double)center.y;
    extra(2, 0) = 0; extra(2, 1) = 0; extra(2, 2) = 1; extra(2, 3) = (double)center.z;
    extra(3, 0) = 0; extra(3, 1) = 0; extra(3, 2) = 0; extra(3, 3) = 1;

    intra(0, 0) = 1; intra(0, 1) = 1; intra(0, 2) = 1;
    intra(1, 0) = 0; intra(1, 1) = 1; intra(1, 2) = 1;
    intra(2, 0) = 0; intra(2, 1) = 0; intra(2, 2) = 1;

    viewer->setCameraParameters(intra, extra);
	
    std::vector<pcl::visualization::Camera> tabCamera;
    viewer->getCameras(tabCamera);
    cam = tabCamera[0];
    PosCamera(cam);

	viewer->setCameraClipDistances(0.1f, 1000.0f);
}

void ResetCamera(void* viewer_void)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer>*>(viewer_void);
	/*viewer->vie*/
    viewer->setCameraParameters(cam);
    viewer->setSize(TAILLE, TAILLE);
	/*viewer->resetCamera();*/
}

void EcritureFichier()
{
    ofstream fichier("datas/cut.txt", ios::out | ios::trunc);
    if (fichier)
    {
        fichier << coupureAxeXcoordZ.size() << endl;
        fichier << coupureAxeYcoordX.size() << endl;
        fichier << coupureAxeZcoordY.size() << endl;

        for (int i = 0; i < coupureAxeXcoordZ.size(); i++)
        {
            fichier << coupureAxeXcoordZ[i] << " ";
        }

        fichier << endl;

        for (int i = 0; i < coupureAxeYcoordX.size(); i++)
        {
            fichier << coupureAxeYcoordX[i] << " ";
        }

        fichier << endl;

        for (int i = 0; i < coupureAxeZcoordY.size(); i++)
        {
            fichier << coupureAxeZcoordY[i] << " ";
        }

        fichier.close();
        std::cout << "ecriture terminee" << std::endl;
    }
    else
    {
        std::cout << "Impossible d'ouvrir le fichier !" << std::endl;
    }
}


void AfficherResultatCoupure()
{
	
    std::string axe;
    int indexTabAxe = 0;

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr tabTemp(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointXYZRGB point;

    if (derniereTouche == 'b')
    {
        ResetCamera((void*)&viewerPost);

        float angle = M_PI;
        extra(0, 0) = cos(angle);  extra(0, 1) = 0; extra(0, 2) = sin(angle);   extra(0, 3) = 0;
        extra(1, 0) = 0;           extra(1, 1) = 1; extra(1, 2) = 0;            extra(1, 3) = 0;
        extra(2, 0) = -sin(angle); extra(2, 1) = 0; extra(2, 2) = cos(angle);   extra(2, 3) = distance;
        extra(3, 0) = 0;           extra(3, 1) = 0; extra(3, 2) = 0;            extra(3, 3) = 1;

        viewerPost->setCameraParameters(intra, extra);
        viewerPost->setSize(TAILLE, TAILLE);

        for (int i = 0; i < face->points.size(); i++)
        {
            point = face->points[i];
            if ((point.z < (p1.z + 0.5f)) && (point.z > (p1.z - 0.5f)))
                tabTemp->points.push_back(point);
        }
    }

    else if (derniereTouche == 'y')
    {
        ResetCamera((void*)&viewerPost);

        float angle = M_PI_2;

        extra(0, 0) = 1; extra(0, 1) = 0;           extra(0, 2) = 0;            extra(0, 3) = 0;
        extra(1, 0) = 0; extra(1, 1) = cos(angle);  extra(1, 2) = -sin(angle);  extra(1, 3) = 0;
        extra(2, 0) = 0; extra(2, 1) = sin(angle);  extra(2, 2) = cos(angle);   extra(2, 3) = 0;
        extra(3, 0) = 0; extra(3, 1) = 0;           extra(3, 2) = 0;            extra(3, 3) = 1;

        Eigen::Matrix4f extra2, extra3;

        extra2(0, 0) = cos(angle);  extra2(0, 1) = 0; extra2(0, 2) = sin(angle);   extra2(0, 3) = -distance;
        extra2(1, 0) = 0;           extra2(1, 1) = 1; extra2(1, 2) = 0;            extra2(1, 3) = 0;
        extra2(2, 0) = -sin(angle); extra2(2, 1) = 0; extra2(2, 2) = cos(angle);   extra2(2, 3) = 0;
        extra2(3, 0) = 0;           extra2(3, 1) = 0; extra2(3, 2) = 0;            extra2(3, 3) = 1;

        extra3 = extra * extra2;

        viewerPost->setCameraParameters(intra, extra3);
        viewerPost->setSize(TAILLE, TAILLE);

        for (int i = 0; i < face->points.size(); i++)
        {
            point = face->points[i];
            if ((point.x < (p1.x + 0.5f)) && (point.x > (p1.x - 0.5f)))
                tabTemp->points.push_back(point);
        }
    }
    else if (derniereTouche == 'z')
    {
        ResetCamera((void*)&viewerPost);

        float angle = -M_PI_2;
        extra(0, 0) = cos(angle);;  extra(0, 1) = -sin(angle);  extra(0, 2) = 0;   extra(0, 3) = 0;
        extra(1, 0) = sin(angle);  extra(1, 1) = cos(angle);    extra(1, 2) = 0;   extra(1, 3) = 0;
        extra(2, 0) = 0;           extra(2, 1) = 0;            extra(2, 2) = 1;   extra(2, 3) = 0;
        extra(3, 0) = 0;           extra(3, 1) = 0;            extra(3, 2) = 0;   extra(3, 3) = 1;

        Eigen::Matrix4f extra2, extra3, extra4, extra5;

        extra2(0, 0) = cos(angle);  extra2(0, 1) = 0; extra2(0, 2) = sin(angle);   extra2(0, 3) = 0;
        extra2(1, 0) = 0;           extra2(1, 1) = 1; extra2(1, 2) = 0;            extra2(1, 3) = 0;
        extra2(2, 0) = -sin(angle); extra2(2, 1) = 0; extra2(2, 2) = cos(angle);   extra2(2, 3) = 0;
        extra2(3, 0) = 0;           extra2(3, 1) = 0; extra2(3, 2) = 0;            extra2(3, 3) = 1;



        extra3 =  extra * extra2;

        viewerPost->setCameraParameters(intra, extra3);

        viewerPost->setSize(TAILLE, TAILLE);

        for (int i = 0; i < face->points.size(); i++)
        {
            point = face->points[i];
            if ((point.y < (p1.y + 0.5f)) && (point.y > (p1.y - 0.5f)))
                tabTemp->points.push_back(point);
        }
    }
	

    viewerPost->removeAllPointClouds();

    viewerPost->addPointCloud<pcl::PointXYZRGB>(tabTemp, "sample" + boost::lexical_cast<std::string>(p1.z));

    viewerPost->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "sample" + boost::lexical_cast<std::string>(p1.z));

    tabTemp->points.clear();

}

void keyboardEventOccurred(const pcl::visualization::KeyboardEvent& event, void* viewer_void)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer>*>(viewer_void);
    
    std::cout << event.getKeySym() << std::endl;
    //TOUCHE a -> quitter le programme
    if (event.getKeySym() == "a" && event.keyDown())
    {
        continuerPreTraitement = false;
        continuerPostTraitement = false;
    }

    else if (event.getKeySym() == "Return" && event.keyDown())
    {
        EcritureFichier();
        continuerPreTraitement = false;
    }

    else if (event.getKeySym() == "Left" && event.keyDown())
    {
        viewer->removeShape(tabLine[tabLine.size() - 1]);
        if (derniereTouche == 'b')
        {
            p1.z += 0.2f;
            p2.z += 0.2f;
            viewer->addLine(p1, p2, 1, 0, 0, derniereLine);
        }
        else if (derniereTouche == 'y')
        {
            p1.x += 0.2f;
            p2.x += 0.2f;
            viewer->addLine(p1, p2, 0, 1, 0, derniereLine);

        }
        else if (derniereTouche == 'z')
        {
            p1.y += 0.2f;
            p2.y += 0.2f;
            viewer->addLine(p1, p2, 0, 0, 1, derniereLine);
        }
        AfficherResultatCoupure();
    }

    else if (event.getKeySym() == "Right" && event.keyDown())
    {
        viewer->removeShape(tabLine[tabLine.size() - 1]);
        if (derniereTouche == 'b')
        {
            p1.z -= 0.2f;
            p2.z -= 0.2f;
            viewer->addLine(p1, p2, 1, 0, 0, derniereLine);
        }
        else if(derniereTouche == 'y')
        {
            p1.x -= 0.2f;
            p2.x -= 0.2f;
            viewer->addLine(p1, p2, 0, 1, 0, derniereLine);

        }
        else if (derniereTouche == 'z')
        {
            p1.y -= 0.2f;
            p2.y -= 0.2f;
            viewer->addLine(p1, p2, 0, 0, 1, derniereLine);
        }
        AfficherResultatCoupure();
    }
    //TOUCHE x -> Caméra axe X
    else if (event.getKeySym() == "b" && event.keyDown() && derniereTouche != 'b')
    {
        ResetCamera((void*)&viewer);

        float angle = -1.5707;
        extra(0, 0) = cos(angle); extra(0, 1) = 0; extra(0, 2) = sin(angle); extra(0, 3) = (double)center.x + distance;
        extra(1, 0) = 0; extra(1, 1) = 1; extra(1, 2) = 0; extra(1, 3) = (double)center.y;
        extra(2, 0) = -sin(angle); extra(2, 1) = 0; extra(2, 2) = cos(angle); extra(2, 3) = (double)center.z;
        extra(3, 0) = 0; extra(3, 1) = 0; extra(3, 2) = 0; extra(3, 3) = 1;

        viewer->setCameraParameters(intra, extra);
        viewer->setSize(TAILLE, TAILLE);
        
        derniereTouche = 'b';
        viewer->updateText("Axe X", 465, 950, 32, 255, 20, 20, "titreAxe");
        
    }
    
    //TOUCHE y -> Caméra axe Y
    else if (event.getKeySym() == "y" && event.keyDown() && derniereTouche!='y')
    {

        ResetCamera((void*)&viewer);

        float angle = 1.5707;

        extra(0, 0) = 1; extra(0, 1) = 0; extra(0, 2) = 0; extra(0, 3) = (double)center.x;
        extra(1, 0) = 0; extra(1, 1) = cos(angle); extra(1, 2) = -sin(angle); extra(1, 3) = (double)center.y + distance;
        extra(2, 0) = 0; extra(2, 1) = sin(angle); extra(2, 2) = cos(angle); extra(2, 3) = (double)center.z;
        extra(3, 0) = 0; extra(3, 1) = 0; extra(3, 2) = 0; extra(3, 3) = 1;

        viewer->setCameraParameters(intra, extra);
        viewer->setSize(TAILLE, TAILLE);

        derniereTouche = 'y';
        viewer->updateText("Axe Y", 465, 950, 32, 255, 20, 20, "titreAxe");

    }

    //TOUCHE z -> Caméra axe Z
    else if (event.getKeySym() == "z" && event.keyDown() && derniereTouche != 'z')
    {
        ResetCamera((void*)&viewer);
   
        float angle = 1.5707 * 2;
		/*Est ce que c'est le bon angle? Je pense pas ...*/
        extra(0, 0) = cos(angle);   extra(0, 1) = 0; extra(0, 2) = sin(angle);      extra(0, 3) = 0;
        extra(1, 0) = 0;            extra(1, 1) = 1; extra(1, 2) = 0;               extra(1, 3) = 0;
        extra(2, 0) = -sin(angle);  extra(2, 1) = 0; extra(2, 2) = cos(angle);      extra(2, 3) = 0;
        extra(3, 0) = 0;            extra(3, 1) = 0; extra(3, 2) = 0;               extra(3, 3) = 1;

        Eigen::Matrix4f extra2,extra3, extra4;

        angle = -1.5707;
        extra2(0, 0) = cos(angle); extra2(0, 1) = -sin(angle) ; extra2(0, 2) = 0; extra2(0, 3) = 0;
        extra2(1, 0) = sin(angle); extra2(1, 1) =  cos(angle) ; extra2(1, 2) = 0; extra2(1, 3) = 0;
        extra2(2, 0) = 0         ; extra2(2, 1) = 0           ; extra2(2, 2) = 1; extra2(2, 3) = 0;
        extra2(3, 0) = 0         ; extra2(3, 1) = 0           ; extra2(3, 2) = 0; extra2(3, 3) = 1;
        
		float angle2 = M_PI;

		extra4(0, 0) = cos(angle2);		extra4(0, 1) = 0;	extra4(0, 2) = sin(angle2);		extra4(0, 3) = 0;
		extra4(1, 0) = 0;				extra4(1, 1) = 1;	extra4(1, 2) = 0;				extra4(1, 3) = 0;
		extra4(2, 0) = -sin(angle2);	extra4(2, 1) = 0;	extra4(2, 2) = cos(angle2);		extra4(2, 3) = 0;
		extra4(3, 0) = 0;				extra4(3, 1) = 0;	extra4(3, 2) = 0;				extra4(3, 3) = 1;

        extra3 = extra4* extra2 * extra;

        viewer->setCameraParameters(intra, extra3);



        viewer->setSize(TAILLE, TAILLE);

        derniereTouche = 'z';
        viewer->updateText("Axe Z", 465, 950, 32, 255, 20, 20, "titreAxe");
    }
}

void pointPickingEventOccurred(const pcl::visualization::PointPickingEvent& event, void* viewer_void)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer>*>(viewer_void);

    float x, y, z;
    if (event.getPointIndex() == -1)
    {
        return;
    }

    event.getPoint(x, y, z);
    std::cout << "Point coordinate ( " << x << ", " << y << ", " << z << ")" << std::endl;
    
    derniereLine = "line" + boost::lexical_cast<std::string>(x) + boost::lexical_cast<std::string>(y) + boost::lexical_cast<std::string>(z) + boost::lexical_cast<std::string>(rand());

    if (derniereTouche == 'b')
    {
        p1.x = maxX;
        p2.x = maxX;

        p1.y = -distance; p1.z = z; p1.r = 127; p1.g = 0; p1.b = 0; p1.a = 255;
        p2.y = distance; p2.z = z; p2.r = 127; p2.g = 0; p2.b = 0; p2.a = 255;
        
        viewer->addLine(p1, p2, 1, 0, 0, derniereLine);
        tabLine.push_back(derniereLine);
        coupureAxeXcoordZ.push_back(p1.z); 
    }
    
    if (derniereTouche == 'y')
    {
        p1.y = maxY;
        p2.y = maxY;

        p1.x = x; p1.z = -distance; p1.r = 127; p1.g = 0; p1.b = 0; p1.a = 255;
        p2.x = x; p2.z = distance; p2.r = 127; p2.g = 0; p2.b = 0; p2.a = 255;

        viewer->addLine(p1, p2, 0, 1, 0, derniereLine);
        tabLine.push_back(derniereLine);
        coupureAxeYcoordX.push_back(p1.x);
    }

    if (derniereTouche == 'z')
    {
        p1.z = maxZ;
        p2.z = maxZ;

        p1.x = -distance; p1.y = y; p1.r = 127; p1.g = 0; p1.b = 0; p1.a = 255;
        p2.x = distance; p2.y = y; p2.r = 127; p2.g = 0; p2.b = 0; p2.a = 255;

        viewer->addLine(p1, p2, 0, 0, 1, derniereLine);
        tabLine.push_back(derniereLine);
        coupureAxeZcoordY.push_back(p1.y);
    }

    AfficherResultatCoupure();
}

void mouseEventOccurred(const pcl::visualization::MouseEvent& event, void* viewer_void)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer>*>(viewer_void);

    if (event.getButton() == pcl::visualization::MouseEvent::RightButton &&
        event.getType() == pcl::visualization::MouseEvent::MouseButtonRelease)
    {
        if (tabLine.size() >= 1)
        {
            viewer->removeShape(tabLine[tabLine.size() - 1]);
            tabLine.pop_back();
        }
    }
}

pcl::PointXYZRGB CenterPCL(pcl::PointCloud<pcl::PointXYZRGB>::Ptr liste)
{
    pcl::PointXYZRGB min = liste->points[0], max = liste->points[0], centre = liste->points[0];

    for (int i = 1; i < liste->points.size(); ++i)
    {
        max.x = (max.x < liste->points[i].x) ? liste->points[i].x : max.x;
        max.y = (max.y < liste->points[i].y) ? liste->points[i].y : max.y;
        max.z = (max.z < liste->points[i].z) ? liste->points[i].z : max.z;

        min.x = (min.x > liste->points[i].x) ? liste->points[i].x : min.x;
        min.y = (min.y > liste->points[i].y) ? liste->points[i].y : min.y;
        min.z = (min.z > liste->points[i].z) ? liste->points[i].z : min.z;
    }

    centre.x = (max.x + min.x) / 2.0f;
    centre.y = (max.y + min.y) / 2.0f;
    centre.z = (max.z + min.z) / 2.0f;

    distance = sqrtf(pow(max.x - min.x, 2) + pow(max.y - min.y, 2) + pow(max.z - min.z, 2));
    std::cout << "Distance : " << sqrtf(pow(max.x - min.x, 2) + pow(max.y - min.y, 2) + pow(max.z - min.z, 2)) << std::endl;
    return centre;
}


void PTStoCloud(std::string path)
{
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr tmp(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::VoxelGrid<pcl::PointXYZRGB> sor;

    ifstream fichier(path);
    bool debut = false;
    int cppt = 0;
    if (fichier)
    {
        std::string ligne;

        while (getline(fichier, ligne))
        {
            if (debut)
            {

                std::istringstream s(ligne);
                std::string test;
                std::vector<std::string> words;
                pcl::PointXYZRGB a;
                
                float temp;
                int n_r, n_g, n_b;
                s >> a.x >> a.y >> a.z >> temp >> n_r >> n_g >> n_b;
                a.r = static_cast<uint8_t>(n_r);
                a.g = static_cast<uint8_t>(n_g);
                a.b = static_cast<uint8_t>(n_b);

				a.x *= 0.1f;
				a.y *= 0.1f;
				a.z *= 0.1f;
                if (a.x < 1000)
                {
                    tmp->points.push_back(a);
                    cppt++;
                }
               
                if (cppt >= 2000000)
                {
                    float x, y, z;
                    std::tie(x, y, z) = DistanceAxe(tmp);
                    double sizeOfLeaf = ((1290 * x) * 9.) / 10.;
                    std::cout << "SizeOfLeaf : " << sizeOfLeaf << std::endl;
                    pcl::octree::OctreePointCloud<pcl::PointXYZRGB> octree(sizeOfLeaf);
                    octree.setInputCloud(tmp);
                    octree.defineBoundingBox();
                    octree.addPointsFromInputCloud();

                    pcl::PointCloud<pcl::PointXYZRGB>::Ptr filtered_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
                    pcl::octree::OctreePointCloud<pcl::PointXYZRGB>::LeafNodeIterator a_it;

                    for (a_it = octree.leaf_depth_begin(); a_it != octree.leaf_depth_end(); ++a_it)
                    {
                        if (a_it.isLeafNode()) {
                            std::vector<int> indexVector;
                            a_it.getLeafContainer().getPointIndices(indexVector);
                            boost::shared_ptr<std::vector<int>> indicesptr(new std::vector<int>(indexVector));

                            *filtered_cloud += *downSample_(tmp, indicesptr, 0.01f, 0.01f, 0.01f);

                        }
                    }
                    *face += *filtered_cloud;
                    tmp->points.clear();
                    cppt = 0;
                }
            }
            else
            {
                debut = true;
                std::cout << "nbr ligne : " << ligne << std::endl;
            }
        }
    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }
}

int main(int argc, char** argv)
{
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
    viewer->setBackgroundColor(0.5f, 0.5f, 0.5f);

    PTStoCloud("EssaiLeger.pts");

    viewer->addPointCloud(face, "face");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "face");

    center = CenterPCL(face);
    viewer->addCoordinateSystem(2.5, center.x, center.y, center.z);

    maxX = CoordMaxX(face);
    maxY = CoordMaxY(face);
    maxZ = CoordMaxZ(face);

    std::cout << "maxX : " << maxX << " maxY : " << maxY << " maxZ : " << maxZ << std::endl;

    InitCamera((void*)&viewer);
    viewer->setCameraParameters(cam);

    float angle = -1.5707;
    extra(0, 0) = cos(angle); extra(0, 1) = 0; extra(0, 2) = sin(angle); extra(0, 3) = (double)center.x + distance;
    extra(1, 0) = 0; extra(1, 1) = 1; extra(1, 2) = 0; extra(1, 3) = (double)center.y;
    extra(2, 0) = -sin(angle); extra(2, 1) = 0; extra(2, 2) = cos(angle); extra(2, 3) = (double)center.z;
    extra(3, 0) = 0; extra(3, 1) = 0; extra(3, 2) = 0; extra(3, 3) = 1;
         
    viewer->setCameraParameters(intra, extra);
    viewer->setSize(TAILLE, TAILLE);

    derniereTouche = 'b';
    viewer->getRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera()->SetParallelProjection(1);
    viewer->addText("Axe X", 465, 950, 32, 255, 20, 20, "titreAxe");

    viewer->registerPointPickingCallback(pointPickingEventOccurred, (void*)&viewer);
    viewer->registerKeyboardCallback(keyboardEventOccurred, (void*)&viewer);
    viewer->registerMouseCallback(mouseEventOccurred, (void*)&viewer);

    viewerPost->setSize(TAILLE, TAILLE);
    viewerPost->setPosition(TAILLE, 0);
    viewerPost->setBackgroundColor(0.5f, 0.5f, 0.5f);

    viewerPost->addCoordinateSystem(2.5, center.x, center.y, center.z);

	ResetCamera((void*)&viewer);
    while(continuerPreTraitement || continuerPostTraitement)
    {
        viewerPost->spinOnce(100);
        viewer->spinOnce(100);

        std::chrono::milliseconds dura(100);
        std::this_thread::sleep_for(dura);
    }

    viewer->close();
    viewerPost->close();
    return 0;
}

