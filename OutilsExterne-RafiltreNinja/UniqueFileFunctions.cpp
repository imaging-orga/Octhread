#include "UniqueFileFunctions.h"
#include <iostream>



void UNIQUE::createXYZFromFile(std::string _dir_name, std::string _file_name, std::string _write_name)
{
	boost::filesystem::path p(_dir_name);
	/*
	 * If all attempted paths exist then bail.
	 */
	if (!boost::filesystem::exists(p))
		boost::filesystem::create_directory(p);

	MyFile file(_file_name);
	std::vector<mypt3d> pts = FileOct::getPtsFromFile(_dir_name, _file_name);

	std::string new_WriteName = p.stem().string() + "\\" + _write_name;
	std::ofstream writeFile(new_WriteName, std::ios::out | std::ios::app);

	for (int i = 0; i < pts.size(); ++i) {
		mypt3d& pt = pts[i];
		writeFile << pt.x << " " << pt.y << " " << pt.z << " " << pt.intensity << " " << (unsigned)pt.r << " " << (unsigned)pt.g << " " << (unsigned)pt.b << "\n";
	}
	writeFile.close();
}

void UNIQUE::createPTSFromFile(std::string _dir_name, std::string _file_name, std::string _write_name) {
	boost::filesystem::path p(_dir_name);
	/*
	 * If all attempted paths exist then bail.
	 */
	if (!boost::filesystem::exists(p))
		boost::filesystem::create_directory(p);


	MyFile file(_file_name);

	std::vector<mypt3d> pts = FileOct::getPtsFromFile(_dir_name, _file_name);
	std::string new_WriteName = p.stem().string() + "\\" + _write_name;

	std::ofstream writeFile(new_WriteName, std::ios::out | std::ios::app);
	writeFile << pts.size();
	for (int i = 0; i < pts.size(); ++i) {
		mypt3d& pt = pts[i];
		writeFile << pt.x << " " << pt.y << " " << pt.z << " " << pt.intensity << " " << (unsigned)pt.r << " " << (unsigned)pt.g << " " << (unsigned)pt.b << "\n";
	}
	writeFile.close();

}

