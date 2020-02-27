//#ifndef __PREVISU_H_
//#define __PREVISU_H_
//#include <iostream>
//#include <pcl/common/common_headers.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include "../Octhread/base.hpp"
//#include "../Octhread/MyFile.h"
//
//struct fileparams {
//	std::string name;
//	int num;
//	pt3d center;
//
//	fileparams(std::string _name, int _num, pt3d _center) :
//		name(_name), num(_num), center(_center) {};
//};
//
//class Previsu
//{
//public:
//	Previsu(std::string& _name);
//	~Previsu();
//
//	/*Fonction pour visualiser un morceau du nuage de points*/
//	void Visualize();
//
//	//Récupérer le vector trié des bons fichiers dans la liste
//	void pickWiseFile();
//private:
//	void SortFileByNum();
//
//
//	void next();
//	void previous();
//	std::string m_name;
//	std::vector<fileparams> m_sortedList;
//	unsigned int m_actual;
//	bool m_valid;
//};
//
//#endif //__PREVISU_H_