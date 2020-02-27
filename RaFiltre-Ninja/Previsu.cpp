//#include "Previsu.h"
//
//Previsu::Previsu(std::string& _name)
//{
//	boost::filesystem::path p(_name); 
//	m_name = p.stem().string();
//
//	m_actual = 0;
//
//	m_valid = true;
//	if (!boost::filesystem::exists(m_name)) {
//		m_valid = false;
//		std::cout << "L'arbre pour le fichier n'a pas été généré. Veuillez le faire s'il vous plait." << std::endl;
//	}
//}
//
//
//Previsu::~Previsu()
//{
//}
//
//
//void keyboardEventOccurred(const pcl::visualization::KeyboardEvent &event,
//	void* viewer_void) 
//{
//	pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *> (viewer_void);
//	if (event.getKeySym() == "q" && event.keyDown())
//	{
//		//previous();
//	}
//	if (event.getKeySym() == "d" && event.keyDown())
//	{
//		//next();
//	}
//}
//
//void mouseEventOccurred(const pcl::visualization::MouseEvent &event,
//	void* viewer_void) 
//{
//	pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *> (viewer_void);
//	if (event.getButton() == pcl::visualization::MouseEvent::LeftButton &&
//		event.getType() == pcl::visualization::MouseEvent::MouseButtonRelease)
//	{
//		std::cout << "Left mouse button released at position (" << event.getX() << ", " << event.getY() << ")" << std::endl;
//
//	}
//}
//
//
//void Previsu::Visualize() {
//	try {
//		if (m_valid) {
//			//On va d'abord récuperer le bon fichier
//			//Puis on va le traiter
//			//Puis on va l'afficher
//
//			//Récuperer le nuage de point a partir du fichier donner 
//
//			//pcl::PointCloud<mypt3d>::ConstPtr cloud = new pcl::PointCloud<mypt3d>();
//			std::string fileToChoose = m_name + "\\" + m_sortedList[m_actual].name;
//			MyFile mf(fileToChoose);
//			std::vector<mypt3d> pts = mf.readFromFile(m_sortedList[m_actual].num);
//
//			pcl::PointCloud<mypt3d>::Ptr cloud = pcl::PointCloud<mypt3d>::Ptr(new pcl::PointCloud<mypt3d>());
//			cloud->width = m_sortedList[m_actual].num;
//			cloud->height = 1;
//			cloud->points.assign(pts.begin(), pts.end());
//
//
//			pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
//			viewer->setBackgroundColor(0, 0, 0);
//			viewer->addPointCloud<mypt3d>(cloud, "sample cloud");
//			viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
//			viewer->addCoordinateSystem(1.0);
//			viewer->initCameraParameters();
//			viewer->setCameraPosition(m_sortedList[m_actual].center.x, m_sortedList[m_actual].center.y, m_sortedList[m_actual].center.z,
//				1, 0, 0, 0);
//			viewer->registerKeyboardCallback(keyboardEventOccurred, (void*)viewer.get());
//			viewer->registerMouseCallback(mouseEventOccurred, (void*)viewer.get());
//			while (!viewer->wasStopped())
//			{
//				viewer->spinOnce(100);
//				//std::this_thread::sleep_for(std::chrono::milliseconds(100));
//			}
//		}
//	}
//	catch(std::exception& e){
//		std::cout << e.what() << " on line : " << __LINE__ << " in file : " << __FILE__ << std::endl;
//	}
//}
//
//void Previsu::pickWiseFile()
//{
//	if (m_valid) {
//		std::string nameOcsave = m_name + "\\.OcSave";
//		std::ifstream file(nameOcsave, std::ios::in);
//
//		std::string line;
//		while (std::getline(file, line)) {
//			std::istringstream ss(line);
//			std::string name;
//			unsigned numPoints;
//
//			float minx, miny, minz;
//			float maxx, maxy, maxz;
//
//
//			ss >> name >> numPoints >> minx >> miny >> minz >> maxx >> maxy >> maxz;
//			pt3d center;
//			center.x = ( minx + maxx) / 2;
//			center.y = ( miny + maxy) / 2;
//			center.z = ( minz + maxz) / 2;
//			if (numPoints != 0)
//				m_sortedList.push_back(fileparams(name, numPoints, center));
//		}
//
//		SortFileByNum();
//	}
//}
//
//inline int distance(int a, int b) {
//	return std::abs(a - b);
//}
//
//void Previsu::SortFileByNum() {
//	int magickNumber = 10000000; //1 million = environ 1go, c'est un bon compromis je pense
//
//	std::sort(m_sortedList.begin(), m_sortedList.end(),
//		[magickNumber](const fileparams& a, fileparams& b) -> bool
//	{
//		return distance(a.num, magickNumber) < distance(b.num, magickNumber);
//	});
//}
//
//void Previsu::next() {
//	if (m_valid) {
//		if (m_actual + 1 < m_sortedList.size()) {
//			m_actual++;
//		}
//	}
//}
//
//void Previsu::previous() {
//	if (m_valid) {
//		if (m_actual > 0) {
//			m_actual--;
//		}
//	}
//}
