#include "UniqueFileFunctions.h"
#include <iostream>
#include <boost/filesystem/convenience.hpp>
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"
#include "lasreader.hpp"
#include <algorithm>
namespace UNIQUE {
	void createXYZFromFile(std::string _dir_name, std::string _file_name, std::string _write_name)
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

	void createPTSFromFile(std::string _dir_name, std::string _file_name, std::string _write_name) {
		boost::filesystem::path p(_dir_name);
		/*
		 * If all attempted paths exist then bail.
		 */

		MyFile file(_file_name);

		std::vector<mypt3d> pts = FileOct::getPtsFromFile(_dir_name, _file_name);
		std::string new_WriteName = /*p.stem().string() + "\\" +*/ _write_name;

		std::ofstream writeFile(new_WriteName, std::ios::out | std::ios::app);
		writeFile << pts.size() << "\n";
		for (int i = 0; i < pts.size() / 8; ++i) {
			mypt3d& pt = pts[i];
			writeFile << pt.x << " " << pt.y << " " << pt.z << " " << pt.intensity << " " << (unsigned)pt.r << " " << (unsigned)pt.g << " " << (unsigned)pt.b << "\n";
		}
		writeFile.close();

	}


	int retrieveInfo(std::string _path, std::string _extension) {
		if (_extension == ".pts") {
			return retrieveInfopts(_path);
		}
		else if (_extension == ".e57") {
			return retrieveInfoe57(_path);
		}
		else if (_extension == ".las" || _extension == ".laz") {
			return retrieveInfolasz(_path);
		}
		return 1;
	}


	int retrieveInfoe57(std::string _path/*e57File*/) {
		//Ouvrir le fichier e57
		//Le header => Regarder combien de scans dedans, le nombre de points, le nombre de fichiers, ...

		// En sortir une string formatée pour pouvoir le récup en c# dans le loader
		// Ou écrire dans un fichier externe?
		// Si il existe pas déjà on le creer et on récupère les infos!
		boost::filesystem::path p(_path);
		std::string nameInfo = "datas\\" + p.stem().string() + ".info"; /*fichier.info*/
		bool existsAlready = (bool)std::ifstream(nameInfo);

		e57::Reader eReader(_path);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		int scanCount = eReader.GetData3DCount();
		int64_t totalPointCount = 0;
		//retrieve total points count
		for (int numScan = 0; numScan < scanCount; ++numScan) {
			e57::Data3D scanHeader;
			eReader.ReadData3D(numScan, scanHeader);

			int64_t nColumn = 0;
			int64_t nRow = 0;

			int64_t nPoints = 0;        //Number of points

			int64_t nGroupsSize = 0;        //Number of groups
			int64_t nCountSize = 0;         //Number of points per group
			bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

			eReader.GetData3DSizes(numScan, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
			totalPointCount += nPoints;
		}


		std::ofstream outFile(nameInfo, std::ofstream::out | std::ofstream::trunc);
		//Ouvrir le fichier e57
		//Ecriture : 
		outFile << p.stem().string() << "\n";
		outFile << scanCount << "\n";
		outFile << totalPointCount << "\n";
		//Nom du fichier
		//nombre de scans
		//Nombre de points totale

		outFile.close();
		return 0;
	}

	int retrieveInfopts(std::string _path)
	{

		unsigned long long int totalPointCount;
		boost::filesystem::path p(_path);
		std::string nameInfo = "datas\\" + p.stem().string() + ".info"; /*fichier.info*/
		bool existsAlready = (bool)std::ifstream(nameInfo);

		std::ifstream inFile(_path, std::ios::in);
		inFile >> totalPointCount;

		inFile.close();

		std::ofstream outFile(nameInfo, std::ios::out | std::ios::trunc);
		outFile << p.stem().string() << "\n";
		outFile << 1 << "\n";
		outFile << totalPointCount << "\n";
		outFile.close();
		return 0;
	}


	int retrieveInfolasz(std::string _path) {
		unsigned long long int totalPointCount;
		boost::filesystem::path p(_path);
		std::string nameInfo = "datas\\" + p.stem().string() + ".info";
		bool existsAlready = (bool)std::ifstream(nameInfo);
		

		LASreadOpener lasreadopener;
		lasreadopener.set_file_name(_path.c_str());

		LASreader* lasreader = lasreadopener.open();

		totalPointCount = lasreader->p_count;
		if (totalPointCount == 0) {
			totalPointCount = lasreader->npoints;
		}

		std::ofstream outFile(nameInfo, std::ios::out | std::ios::trunc);
		outFile << p.stem().string() << "\n";
		outFile << 1 << "\n";
		outFile << totalPointCount << "\n";
		outFile.close();
		lasreader->close();
		return 0;
	}
}