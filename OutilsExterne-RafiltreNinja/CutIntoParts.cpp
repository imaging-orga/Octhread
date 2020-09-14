#include "CutIntoParts.h"
#include "../Octhread/MyFile.h"
#include "laswriter.hpp"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"
#include "laszip.hpp"
#include "lasreader.hpp"


CutIntoParts::~CutIntoParts()
{
}

/*
xNum
yNum
zNum
X1,X2,X3,...,xN
Y1,Y2,Y3,...,yN
Z1,Z2,Z3,...,zN
*/
CutIntoParts::CutIntoParts(std::string _filename, std::string _retExt, std::string _cutFile, float _overlap) :
	m_name(_filename), m_overlap(_overlap), m_retExt(_retExt) {
	
	boost::filesystem::path p(_filename);
	tempNameFolder = p.parent_path().string() + "\\" + p.stem().string() + "-temp"; //for the temporary files
	CreateDirectory(tempNameFolder.c_str(), NULL);
	applyCutFile(_filename, _cutFile);
	cutFileIncluded = true;
	nameCutter = p/*.stem()*/.string() + "-cutter";
	CreateDirectory(nameCutter.c_str(), NULL);
	//origin = (0,0,0)
}



CutIntoParts::CutIntoParts(std::string _file, std::string _retExt, float _xSize, float _ySize, float _zSize, float _overlap, cv::Vec3f _xAxis, cv::Vec3f _yAxis, cv::Vec3f _zAxis) :
	m_name(_file), m_retExt(_retExt), m_xSize(_xSize), m_ySize(_ySize), m_zSize(_zSize),
	m_xAxis(_xAxis), m_yAxis(_yAxis), m_zAxis(_zAxis),
	m_overlap(_overlap)
{
	boost::filesystem::path p(_file);
	tempNameFolder = p.parent_path().string() +"\\"+ p.stem().string() + "-temp"; //for the temporary files
	CreateDirectory(tempNameFolder.c_str(), NULL);

	nameCutter = p/*.stem()*/.string() + "-cutter";
	BoundingBox bb = getBB();
	offset = bb.min;
	float sizeX, sizeY, sizeZ;
	sizeX = bb.max.x - bb.min.x;
	sizeY = bb.max.y - bb.min.y;
	sizeZ = bb.max.z - bb.min.z;
	//sizeX = bb.getHalfDimension().x * 2;
	//sizeY = bb.getHalfDimension().y * 2;
	//sizeZ = bb.getHalfDimension().z * 2;

	//On assume que les axes sont (1,0,0) (0,1,0) (0,0,1)
	m_xNum = (sizeX / _xSize) + 1;
	m_yNum = (sizeY / _ySize) + 1;
	m_zNum = (sizeZ / _zSize) + 1;

	for (int x = 0; x < m_xNum; ++x) {
		for (int y = 0; y < m_yNum; ++y) {
			for (int z = 0; z < m_zNum; ++z) {
				BoundingBox bb;
				pt3d min; 
				min.x = m_xSize * x;
				min.y = m_ySize * y;
				min.z = m_zSize * z;
				pt3d max;
				max.x = m_xSize * (x+1);
				max.y = m_ySize * (y+1);
				max.z = m_zSize * (z+1);
				bb.min = min; bb.max = max;
				Part p(tempNameFolder, x, y, z,bb, m_xSize, m_ySize, m_zSize, offset);
				parts.push_back(p);
			}
		}
	}
	CreateDirectory(nameCutter.c_str(), NULL);

	cutFileIncluded = false;
	
}


void CutIntoParts::applyCutFile(std::string _filename, std::string _cutName)
{
	boost::filesystem::path p(_filename);
	tempNameFolder = p.parent_path().string() + "\\" + p.stem().string() + "-temp"; //for the temporary files
	//ajouter les bonnes parts
	std::ifstream cutFile(_cutName, std::ios::in);
	std::string line;
	std::istringstream ss;
	std::getline(cutFile, line); //X
	ss = std::istringstream(line);
	ss >> m_xNum;
	std::getline(cutFile, line); //Y
	ss = std::istringstream(line);
	ss >> m_yNum;
	std::getline(cutFile, line); //Z 
	ss = std::istringstream(line);
	ss >> m_zNum;

	

	std::getline(cutFile, line); //X
	ss = std::istringstream(line);
	for (size_t i = 0; i < m_xNum; ++i) {
		float num;
		ss >> num;
		fileSizeX.push_back(num);
	}

	std::getline(cutFile, line); //Y
	ss = std::istringstream(line);
	for (size_t i = 0; i < m_yNum; ++i) {
		float num;
		ss >> num;
		fileSizeY.push_back(num);
	}

	std::getline(cutFile, line); //Z
	ss = std::istringstream(line);
	for (size_t i = 0; i < m_zNum; ++i) {
		float num;
		ss >> num;
		fileSizeZ.push_back(num);
	}


	for (int x = 0; x < m_xNum-1; ++x) {
		for (int y = 0; y < m_yNum-1; ++y) {
			for (int z = 0; z < m_zNum-1; ++z) {
				pt3d min;
				min.x = fileSizeX[x];
				min.y = fileSizeY[y];
				min.z = fileSizeZ[z];
				pt3d max;
				max.x = fileSizeX[x+1];
				max.y = fileSizeY[y+1];
				max.z = fileSizeZ[z+1];

				BoundingBox bb(min, max);


				Part p(tempNameFolder, x, y, z, bb, fileSizeX[x], fileSizeY[y], fileSizeZ[z], offset);
				parts.push_back(p);
			}
		}
	}

	cutFile.close();
}

BoundingBox CutIntoParts::getBB() {
	boost::filesystem::path p(m_name);
	std::string ext = "";
	if (p.has_extension()) {
		ext = p.extension().string();
	}

	if (ext == ".e57") {
		BoundingBox bb = getBBe57();
		return bb;
	}
	else if (ext == ".pts") {
		return getBBPts();
	}
	else if (ext == ".las" || ext == ".laz") {
		return getBBLasLaz();
	}
	else return BoundingBox();

}

BoundingBox CutIntoParts::getBBe57() {
	BoundingBox ret;
	e57::Reader eReader(m_name);
	int numberOfScan = eReader.GetData3DCount();
	//Premier scan
	double xMinFinal, yMinFinal, zMinFinal;
	double xMaxFinal, yMaxFinal, zMaxFinal;

	double xMin_tmp, yMin_tmp, zMin_tmp;
	double xMax_tmp, yMax_tmp, zMax_tmp;

	e57::Data3D scanHeader1;
	eReader.ReadData3D(0, scanHeader1);
	double xTrans = scanHeader1.pose.translation.x;
	double yTrans = scanHeader1.pose.translation.y;
	double zTrans = scanHeader1.pose.translation.z;
	double wRot = scanHeader1.pose.rotation.w;
	double xRot = scanHeader1.pose.rotation.x;
	double yRot = scanHeader1.pose.rotation.y;
	double zRot = scanHeader1.pose.rotation.z;

	double r00, r10, r20;
	double r01, r11, r21;
	double r02, r12, r22;
	r00 = 1. - 2. * yRot * yRot - 2. * zRot * zRot;
	r10 = 2. * xRot * yRot - 2. * zRot * wRot;
	r20 = 2. * xRot * zRot + 2. * yRot * wRot;

	r01 = 2. * xRot *yRot + 2. * zRot * wRot;
	r11 = 1. - 2. * xRot * xRot - 2. * zRot * zRot;
	r21 = 2. * yRot * zRot - 2. * xRot * wRot;

	r02 = 2. * xRot * zRot - 2. * yRot * wRot;
	r12 = 2. * yRot * zRot + 2. * xRot * wRot;
	r22 = 1. - 2. * xRot * xRot - 2. * yRot * yRot;


	xMin_tmp = scanHeader1.cartesianBounds.xMinimum;
	xMax_tmp = scanHeader1.cartesianBounds.xMaximum;

	yMin_tmp = scanHeader1.cartesianBounds.yMinimum;
	yMax_tmp = scanHeader1.cartesianBounds.yMaximum;

	zMin_tmp = scanHeader1.cartesianBounds.zMinimum;
	zMax_tmp = scanHeader1.cartesianBounds.zMaximum;


	xMinFinal = xMin_tmp * r00 + yMin_tmp * r10 + zMin_tmp * r20 + xTrans;
	yMinFinal = xMin_tmp * r01 + yMin_tmp * r11 + zMin_tmp * r21 + yTrans;
	zMinFinal = xMin_tmp * r02 + yMin_tmp * r12 + zMin_tmp * r22 + zTrans;

	xMaxFinal = xMax_tmp * r00 + yMax_tmp * r10 + zMax_tmp * r20 + xTrans;
	yMaxFinal = xMax_tmp * r01 + yMax_tmp * r11 + zMax_tmp * r21 + yTrans;
	zMaxFinal = xMax_tmp * r02 + yMax_tmp * r12 + zMax_tmp * r22 + zTrans;


	//Si il y en a plus :
	if (numberOfScan > 1) {
		for (int i = 1; i < numberOfScan; ++i) {
			e57::Data3D scanHeader;
			eReader.ReadData3D(i, scanHeader);


			xTrans = scanHeader.pose.translation.x;
			yTrans = scanHeader.pose.translation.y;
			zTrans = scanHeader.pose.translation.z;
			wRot = scanHeader.pose.rotation.w;
			xRot = scanHeader.pose.rotation.x;
			yRot = scanHeader.pose.rotation.y;
			zRot = scanHeader.pose.rotation.z;
			r00 = 1. - 2. * yRot * yRot - 2. * zRot * zRot;
			r10 = 2. * xRot * yRot - 2. * zRot * wRot;
			r20 = 2. * xRot * zRot + 2. * yRot * wRot;

			r01 = 2. * xRot *yRot + 2. * zRot * wRot;
			r11 = 1. - 2. * xRot * xRot - 2. * zRot * zRot;
			r21 = 2. * yRot * zRot - 2. * xRot * wRot;

			r02 = 2. * xRot * zRot - 2. * yRot * wRot;
			r12 = 2. * yRot * zRot + 2. * xRot * wRot;
			r22 = 1. - 2. * xRot * xRot - 2. * yRot * yRot;
			xMin_tmp = scanHeader.cartesianBounds.xMinimum;
			xMax_tmp = scanHeader.cartesianBounds.xMaximum;

			yMin_tmp = scanHeader.cartesianBounds.yMinimum;
			yMax_tmp = scanHeader.cartesianBounds.yMaximum;

			zMin_tmp = scanHeader.cartesianBounds.zMinimum;
			zMax_tmp = scanHeader.cartesianBounds.zMaximum;

			double xMin, yMin, zMin;
			double xMax, yMax, zMax;


			xMin = xMin_tmp * r00 + yMin_tmp * r10 + zMin_tmp * r20 + xTrans;
			yMin = xMin_tmp * r01 + yMin_tmp * r11 + zMin_tmp * r21 + yTrans;
			zMin = xMin_tmp * r02 + yMin_tmp * r12 + zMin_tmp * r22 + zTrans;

			xMax = xMax_tmp * r00 + yMax_tmp * r10 + zMax_tmp * r20 + xTrans;
			yMax = xMax_tmp * r01 + yMax_tmp * r11 + zMax_tmp * r21 + yTrans;
			zMax = xMax_tmp * r02 + yMax_tmp * r12 + zMax_tmp * r22 + zTrans;

			if (xMin < xMinFinal)
				xMinFinal = xMin;
			if (xMax > xMaxFinal)
				xMaxFinal = xMax;

			if (yMin < yMinFinal)
				yMinFinal = yMin;
			if (yMax > yMaxFinal)
				yMaxFinal = yMax;

			if (zMin < zMinFinal)
				zMinFinal = zMin;
			if (zMax > zMaxFinal)
				zMaxFinal = zMax;
		}
	}

	//Si les dimensions sont [-inf, inf] => Lire tous les points une fois pour connaître la taille de la boite englobante

	if (xMinFinal < 10e-6 || yMinFinal < 10e-6 || zMinFinal < 10e-6
		|| xMaxFinal > 10e6 || yMaxFinal > 10e6 || zMaxFinal > 10e6) {
		std::pair<pt3d, pt3d> minMax = read_NonUnifiedForBoundingBox();
		ret.min = minMax.first;
		ret.max = minMax.second;

	}
	else {

		ret.min = pt3d(xMinFinal, yMinFinal, zMinFinal);
		ret.max = pt3d(xMaxFinal, yMaxFinal, zMaxFinal);
	}

	//std::pair<pt3d, pt3d> minMax = read_NonUnifiedForBoundingBox();

	return ret;
}

std::pair<pt3d, pt3d> CutIntoParts::read_NonUnifiedForBoundingBox() {
	e57::Reader eReader(m_name);
	e57::E57Root rootHeader;
	eReader.GetE57Root(rootHeader);
	std::string guid = rootHeader.guid;
	std::string coordinateMetaData = rootHeader.coordinateMetadata;
	int numberOfScan = eReader.GetData3DCount();
	pt3d min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	pt3d max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	for (int numScan = 0; numScan < numberOfScan; ++numScan) {
		e57::Data3D scanHeader;
		eReader.ReadData3D(numScan, scanHeader);
		int64_t sizeChunks = 1024 * 1024 * 128;

		int64_t nColumn = 0;
		int64_t nRow = 0;

		int64_t nPoints = 0;        //Number of points

		int64_t nGroupsSize = 0;        //Number of groups
		int64_t nCountSize = 0;         //Number of points per group
		bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

		eReader.GetData3DSizes(numScan, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);

		int64_t nSize = nRow;

		if (nRow == 0 && nColumn == 0) {
			nRow = nPoints;
			nColumn = 1;

			nGroupsSize = 1;
			nGroupsSize = 1;
			nCountSize = nPoints;
			bColumnIndex = true;
		}

		double xTrans = scanHeader.pose.translation.x;
		double yTrans = scanHeader.pose.translation.y;
		double zTrans = scanHeader.pose.translation.z;

		double wRot = scanHeader.pose.rotation.w;
		double xRot = scanHeader.pose.rotation.x;
		double yRot = scanHeader.pose.rotation.y;
		double zRot = scanHeader.pose.rotation.z;

		std::vector<double> xData, yData, zData;
		if (scanHeader.pointFields.cartesianXField)
			xData.resize(sizeChunks);

		if (scanHeader.pointFields.cartesianYField)
			yData.resize(sizeChunks);

		if (scanHeader.pointFields.cartesianZField)
			zData.resize(sizeChunks);


		std::vector<int32_t> rowIndex;
		std::vector<int32_t> columnIndex;
		if (scanHeader.pointFields.rowIndexField)
			rowIndex.resize(sizeChunks);
		if (scanHeader.pointFields.columnIndexField)
			columnIndex.resize(sizeChunks);
		e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
			numScan,                      //!< data block index given by the NewData3D
			sizeChunks,                           //!< size of each of the buffers given
			xData.data(),                          //!< pointer to a buffer with the x data
			yData.data(),                          //!< pointer to a buffer with the y data
			zData.data(),                          //!< pointer to a buffer with the z data
			NULL,          //!< pointer to a buffer with the valid indication
			NULL,                        //!< pointer to a buffer with the lidar return intesity
			NULL,
			NULL,                        //!< pointer to a buffer with the color red data
			NULL,                      //!< pointer to a buffer with the color green data
			NULL,                       //!< pointer to a buffer with the color blue data
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			rowIndex.data(),                       //!< pointer to a buffer with the rowIndex
			columnIndex.data()                     //!< pointer to a buffer with the columnIndex
		);


		double r00, r10, r20;
		double r01, r11, r21;
		double r02, r12, r22;
		r00 = 1. - 2. * yRot * yRot - 2. * zRot * zRot;
		r10 = 2. *  xRot * yRot - 2. * zRot *  wRot;
		r20 = 2. *  xRot *  zRot + 2. *  yRot *  wRot;

		r01 = 2. *  xRot * yRot + 2. *  zRot *  wRot;
		r11 = 1. - 2. *  xRot *  xRot - 2. *  zRot *  zRot;
		r21 = 2. *  yRot *  zRot - 2. *  xRot *  wRot;

		r02 = 2. *  xRot *  zRot - 2. *  yRot *  wRot;
		r12 = 2. *  yRot *  zRot + 2. *  xRot *  wRot;
		r22 = 1. - 2. *  xRot *  xRot - 2. *  yRot *  yRot;


		unsigned size = 0;
		size_t col = 0;
		size_t  row = 0;
		int64_t count = 0;
		mypt3d center(xTrans, yTrans, zTrans, 0, 0, 0, 0);
		//Faire la distinction avec une demande de filtre sur la distance ou non.
		while (size = dataReader.read()) {
			std::vector<mypt3d> pts;
			for (int64_t i = 0; i < size; ++i) {
				if (columnIndex.data())
					col = columnIndex[i];
				else
					col = 0;        //point cloud case

				if (rowIndex.data())
					row = rowIndex[i];
				else
					row = count;    //point cloud case

				float x, y, z;
				x = xData[i] * r00 + yData[i] * r10 + zData[i] * r20 + xTrans;
				y = xData[i] * r01 + yData[i] * r11 + zData[i] * r21 + yTrans;
				z = xData[i] * r02 + yData[i] * r12 + zData[i] * r22 + zTrans;

				if (x < min.x)
					min.x = x;
				if (x > max.x)
					max.x = x;
				if (y < min.y)
					min.y = y;
				if (y > max.y)
					max.y = y;
				if (z < min.z)
					min.z = z;
				if (z > max.z)
					max.z = z;
			}
		}
	}
	eReader.Close();
	return std::pair<pt3d, pt3d>(min, max);
}
BoundingBox CutIntoParts::getBBLasLaz() {
	LASreadOpener lasreadopener;
	lasreadopener.set_file_name(m_name.c_str());
	LASreader* lasreader = lasreadopener.open();
	double minX = lasreader->get_min_x();
	double minY = lasreader->get_min_y();
	double minZ = lasreader->get_min_z();
	double maxX = lasreader->get_max_x();
	double maxY = lasreader->get_max_y();
	double maxZ = lasreader->get_max_z();


	//read tous les points
	if (minX == 0.0 && minY == 0.0 && minZ == 0.0 && maxX == 0.0 && maxY == 0.0 && maxZ == 0.0) { // aucune donnée, on va devoir le calculé...

		minX = std::numeric_limits<double>::max();
		minY = std::numeric_limits<double>::max();
		minZ = std::numeric_limits<double>::max();

		maxX = std::numeric_limits<double>::min();
		maxY = std::numeric_limits<double>::min();
		maxZ = std::numeric_limits<double>::min();
		while (lasreader->read_point()) { //Long!! mais si la donnée n'est pas calculé, c'est la hess
			LASpoint& laspt = lasreader->point;

			if (laspt.get_x() < minX)
				minX = laspt.get_x();
			if (laspt.get_y() < minY)
				minY = laspt.get_y();
			if (laspt.get_z() < minZ)
				minZ = laspt.get_z();


			if (laspt.get_x() > maxX)
				maxX = laspt.get_x();
			if (laspt.get_y() > maxY)
				maxY = laspt.get_y();
			if (laspt.get_z() > maxZ)
				maxZ = laspt.get_z();
		}

	}

	return BoundingBox(pt3d(minX, minY, minZ), pt3d(maxX, maxY, maxZ));

}
BoundingBox CutIntoParts::getBBPts() {
	//long int bufferSize = 1024 * 1024 * 128;
	std::ifstream file(m_name, std::ios::in);
	float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max(), minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min(), maxY = std::numeric_limits<float>::min(), maxZ = std::numeric_limits<float>::min();
	std::string line;
	std::getline(file, line);
	long unsigned maxLine = std::atol(line.c_str());

	for (int i = 0; i < maxLine; ++i) {
		std::getline(file, line);
		std::istringstream ss(line);
		float tmpX, tmpY, tmpZ;
		ss >> tmpX >> tmpY >> tmpZ;
		if (tmpX < minX)
			minX = tmpX;
		if (tmpY < minY)
			minY = tmpY;
		if (tmpZ < minZ)
			minZ = tmpZ;

		if (tmpX > maxX)
			maxX = tmpX;
		if (tmpY > maxY)
			maxY = tmpY;
		if (tmpZ > maxZ)
			maxZ = tmpZ;
	}

	file.close();
	return BoundingBox(pt3d(minX, minY, minZ), pt3d(maxX, maxY, maxZ));
}

Part& CutIntoParts::getPart(unsigned x, unsigned y, unsigned z)
{
	int index = x * m_yNum * m_zNum + y * m_zNum + z;
	return parts[index];
}

void CutIntoParts::readFile()
{
	boost::filesystem::path p(m_name);
	std::string ext = "";
	if (p.has_extension()) {
		ext = p.extension().string();
	}

	if (ext == ".e57") {
		readE57();
	}
	else if (ext == ".pts") {
		readPTS();
	}
	else if (ext == ".las" || ext == ".laz") {
		readLasLaz();
	}
}

/// <summary>
/// Créer un sizeChunk
/// Remplir un vector de point de taille [sizeChunk]
/// ajouter ces points avec addVector
/// Au bout d'un certain nombre de points [bufferLimit] on mets a jour et on ajoute les oints dans les fichier.
/// Se faisant, on remet tout a 0
/// 
/// </summary>
void CutIntoParts::readE57() {
	unsigned sizeChunks = 1024 * 1024 * 128;
	//std::vector < mypt3d > pts;
	//pts.resize(sizeChunks);

	unsigned long long bufferLimit = sizeChunks;

	unsigned long long nPointsCpt = 0;
	//read bufferSize points

	e57::Reader eReader(m_name);
	e57::E57Root rootHeader;
	eReader.GetE57Root(rootHeader);
	std::string guid = rootHeader.guid;
	std::string coordinateMetaData = rootHeader.coordinateMetadata;
	int numberOfScan = eReader.GetData3DCount();


	if (numberOfScan == 1) { // Unifié
		e57::Data3D scanHeader;
		eReader.ReadData3D(0, scanHeader);

		int64_t nColumn = 0;
		int64_t nRow = 0;

		int64_t nPoints = 0;        //Number of points

		int64_t nGroupsSize = 0;        //Number of groups
		int64_t nCountSize = 0;         //Number of points per group
		bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"
		eReader.GetData3DSizes(0, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);

		int64_t nSize = nRow;
		double xTrans = scanHeader.pose.translation.x;
		double yTrans = scanHeader.pose.translation.y;
		double zTrans = scanHeader.pose.translation.z;

		double wRot = scanHeader.pose.rotation.w;
		double xRot = scanHeader.pose.rotation.x;
		double yRot = scanHeader.pose.rotation.y;
		double zRot = scanHeader.pose.rotation.z;
		if (nRow == 0 && nColumn == 0) {
			nRow = nPoints;
			nColumn = 1;

			nGroupsSize = 1;
			nGroupsSize = 1;
			nCountSize = nPoints;
			bColumnIndex = true;
		}
		std::vector<int8_t> isInvalidData;
		bool bInvalidState;

		if (scanHeader.pointFields.cartesianInvalidStateField) {
			isInvalidData.resize(sizeChunks);
			bInvalidState = true;
		}



		std::vector<double> xData, yData, zData;
		if (scanHeader.pointFields.cartesianXField)
			xData.resize(sizeChunks);

		if (scanHeader.pointFields.cartesianYField)
			yData.resize(sizeChunks);

		if (scanHeader.pointFields.cartesianZField)
			zData.resize(sizeChunks);


		bool bIntens = false;
		std::vector<double>intData;
		double intRange = 0;
		double intOffset = 0;
		if (scanHeader.pointFields.intensityField) {
			bIntens = true;
			intData.resize(sizeChunks);
			intRange = scanHeader.intensityLimits.intensityMaximum - scanHeader.intensityLimits.intensityMinimum;
			intOffset = scanHeader.intensityLimits.intensityMinimum;
		}

		bool bColor = false;
		std::vector<uint16_t> redData, greenData, blueData;
		double colorRedRange = 1;
		double colorRedOffset = 0;
		double colorGreenRange = 1;
		double colorGreenOffset = 0;
		double colorBlueRange = 1;
		double colorBlueOffset = 0;
		if (scanHeader.pointFields.colorRedField)
		{
			bColor = true;
			redData.resize(sizeChunks);
			greenData.resize(sizeChunks);
			blueData.resize(sizeChunks);
			colorRedRange = scanHeader.colorLimits.colorRedMaximum - scanHeader.colorLimits.colorRedMinimum;
			colorRedOffset = scanHeader.colorLimits.colorRedMinimum;
			colorGreenRange = scanHeader.colorLimits.colorGreenMaximum - scanHeader.colorLimits.colorGreenMinimum;
			colorGreenOffset = scanHeader.colorLimits.colorGreenMinimum;
			colorBlueRange = scanHeader.colorLimits.colorBlueMaximum - scanHeader.colorLimits.colorBlueMinimum;
			colorBlueOffset = scanHeader.colorLimits.colorBlueMinimum;
		}

		std::vector<int64_t> idElementValue;
		std::vector<int64_t> startPointIndex;
		std::vector<int64_t> pointCount;
		if (nGroupsSize > 0)
		{
			idElementValue.resize(nGroupsSize);
			startPointIndex.resize(nGroupsSize);
			pointCount.resize(nGroupsSize);

			if (!eReader.ReadData3DGroupsData(0, nGroupsSize, idElementValue.data(),
				startPointIndex.data(), pointCount.data()))
				nGroupsSize = 0;
		}



		std::vector<int32_t> rowIndex;
		std::vector<int32_t> columnIndex;
		if (scanHeader.pointFields.rowIndexField)
			rowIndex.resize(sizeChunks);
		if (scanHeader.pointFields.columnIndexField)
			columnIndex.resize(sizeChunks);
		e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
			0,                      //!< data block index given by the NewData3D
			sizeChunks,                           //!< size of each of the buffers given
			xData.data(),                          //!< pointer to a buffer with the x data
			yData.data(),                          //!< pointer to a buffer with the y data
			zData.data(),                          //!< pointer to a buffer with the z data
			isInvalidData.data(),          //!< pointer to a buffer with the valid indication
			intData.data(),                        //!< pointer to a buffer with the lidar return intesity
			NULL,
			redData.data(),                        //!< pointer to a buffer with the color red data
			greenData.data(),                      //!< pointer to a buffer with the color green data
			blueData.data(),                       //!< pointer to a buffer with the color blue data
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			rowIndex.data(),                       //!< pointer to a buffer with the rowIndex
			columnIndex.data()                     //!< pointer to a buffer with the columnIndex
		);
		double r00, r10, r20;
		double r01, r11, r21;
		double r02, r12, r22;
		r00 = 1. - 2. * yRot * yRot - 2. * zRot * zRot;
		r10 = 2. *  xRot * yRot - 2. * zRot *  wRot;
		r20 = 2. *  xRot *  zRot + 2. *  yRot *  wRot;

		r01 = 2. *  xRot * yRot + 2. *  zRot *  wRot;
		r11 = 1. - 2. *  xRot *  xRot - 2. *  zRot *  zRot;
		r21 = 2. *  yRot *  zRot - 2. *  xRot *  wRot;

		r02 = 2. *  xRot *  zRot - 2. *  yRot *  wRot;
		r12 = 2. *  yRot *  zRot + 2. *  xRot *  wRot;
		r22 = 1. - 2. *  xRot *  xRot - 2. *  yRot *  yRot;
		unsigned size = 0;
		size_t col = 0;
		size_t  row = 0;
		int64_t count = 0;
		unsigned long cptTmp = 0;
		while (size = dataReader.read()) {
			std::vector<mypt3d> pts;
			pts.resize(size);
			for (int64_t i = 0; i < size; ++i) {
				if (columnIndex.data())
					col = columnIndex[i];
				else
					col = 0;        //point cloud case

				if (rowIndex.data())
					row = rowIndex[i];
				else
					row = count;    //point cloud case

				mypt3d pt;
				pt.x = xData[i] * r00 + yData[i] * r10 + zData[i] * r20 + xTrans;
				pt.y = xData[i] * r01 + yData[i] * r11 + zData[i] * r21 + yTrans;
				pt.z = xData[i] * r02 + yData[i] * r12 + zData[i] * r22 + zTrans;
				if (bIntens) {         //Normalize intensity to 0 - 1.
					double intensity;
					if (intRange != 0)
						intensity = (intData[i] - intOffset) / intRange;
					else
						intensity = intData[i];
					pt.intensity = intensity;
				}
				else {
					pt.intensity = 0.5;
				}

				if (bColor) {                     //Normalize color to 0 - 255
					int red = (int)(((redData[i] - colorRedOffset) * 255) / colorRedRange);
					int green = (int)(((greenData[i] - colorGreenOffset) * 255) / colorGreenRange);
					int blue = (int)(((blueData[i] - colorBlueOffset) * 255) / colorBlueRange);
					pt.r = red;
					pt.g = green;
					pt.b = blue;
				}
				else {
					pt.r = pt.intensity * 255;
					pt.g = pt.intensity * 255;
					pt.b = pt.intensity * 255;
				}

				pts[i] = pt;
				count++;
				nPointsCpt++;
				cptTmp++;
			}
			addVector(pts);
			if (cptTmp >= bufferLimit || count > nPoints - 5) {

				writeInFiles();
				cptTmp = 0;
			}
			nPointsCpt = 0;
		}
	}
	else { //Non unifié
		for (int numScan = 0; numScan < numberOfScan; ++numScan) {
			e57::Data3D scanHeader;
			eReader.ReadData3D(numScan, scanHeader);
			int64_t sizeChunks = 1024 * 1024 * 10;

			int64_t nColumn = 0;
			int64_t nRow = 0;

			int64_t nPoints = 0;        //Number of points

			int64_t nGroupsSize = 0;        //Number of groups
			int64_t nCountSize = 0;         //Number of points per group
			bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

			eReader.GetData3DSizes(numScan, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);

			int64_t nSize = nRow;

			if (nRow == 0 && nColumn == 0) {
				nRow = nPoints;
				nColumn = 1;

				nGroupsSize = 1;
				nGroupsSize = 1;
				nCountSize = nPoints;
				bColumnIndex = true;
			}

			double xTrans = scanHeader.pose.translation.x;
			double yTrans = scanHeader.pose.translation.y;
			double zTrans = scanHeader.pose.translation.z;

			double wRot = scanHeader.pose.rotation.w;
			double xRot = scanHeader.pose.rotation.x;
			double yRot = scanHeader.pose.rotation.y;
			double zRot = scanHeader.pose.rotation.z;

			std::vector<int8_t> isInvalidData;
			bool bInvalidState;

			if (scanHeader.pointFields.cartesianInvalidStateField) {
				isInvalidData.resize(sizeChunks);
				bInvalidState = true;
			}



			std::vector<double> xData, yData, zData;
			if (scanHeader.pointFields.cartesianXField)
				xData.resize(sizeChunks);

			if (scanHeader.pointFields.cartesianYField)
				yData.resize(sizeChunks);

			if (scanHeader.pointFields.cartesianZField)
				zData.resize(sizeChunks);


			bool bIntens = false;
			std::vector<double>intData;
			double intRange = 0;
			double intOffset = 0;
			if (scanHeader.pointFields.intensityField) {
				bIntens = true;
				intData.resize(sizeChunks);
				intRange = scanHeader.intensityLimits.intensityMaximum - scanHeader.intensityLimits.intensityMinimum;
				intOffset = scanHeader.intensityLimits.intensityMinimum;
			}

			bool bColor = false;
			std::vector<uint16_t> redData, greenData, blueData;
			double colorRedRange = 1;
			double colorRedOffset = 0;
			double colorGreenRange = 1;
			double colorGreenOffset = 0;
			double colorBlueRange = 1;
			double colorBlueOffset = 0;
			if (scanHeader.pointFields.colorRedField)
			{
				bColor = true;
				redData.resize(sizeChunks);
				greenData.resize(sizeChunks);
				blueData.resize(sizeChunks);
				colorRedRange = scanHeader.colorLimits.colorRedMaximum - scanHeader.colorLimits.colorRedMinimum;
				colorRedOffset = scanHeader.colorLimits.colorRedMinimum;
				colorGreenRange = scanHeader.colorLimits.colorGreenMaximum - scanHeader.colorLimits.colorGreenMinimum;
				colorGreenOffset = scanHeader.colorLimits.colorGreenMinimum;
				colorBlueRange = scanHeader.colorLimits.colorBlueMaximum - scanHeader.colorLimits.colorBlueMinimum;
				colorBlueOffset = scanHeader.colorLimits.colorBlueMinimum;
			}

			std::vector<int64_t> idElementValue;
			std::vector<int64_t> startPointIndex;
			std::vector<int64_t> pointCount;
			if (nGroupsSize > 0)
			{
				idElementValue.resize(nGroupsSize);
				startPointIndex.resize(nGroupsSize);
				pointCount.resize(nGroupsSize);

				if (!eReader.ReadData3DGroupsData(numScan, nGroupsSize, idElementValue.data(),
					startPointIndex.data(), pointCount.data()))
					nGroupsSize = 0;
			}



			std::vector<int32_t> rowIndex;
			std::vector<int32_t> columnIndex;
			if (scanHeader.pointFields.rowIndexField)
				rowIndex.resize(sizeChunks);
			if (scanHeader.pointFields.columnIndexField)
				columnIndex.resize(sizeChunks);
			e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
				numScan,                      //!< data block index given by the NewData3D
				sizeChunks,                           //!< size of each of the buffers given
				xData.data(),                          //!< pointer to a buffer with the x data
				yData.data(),                          //!< pointer to a buffer with the y data
				zData.data(),                          //!< pointer to a buffer with the z data
				isInvalidData.data(),          //!< pointer to a buffer with the valid indication
				intData.data(),                        //!< pointer to a buffer with the lidar return intesity
				NULL,
				redData.data(),                        //!< pointer to a buffer with the color red data
				greenData.data(),                      //!< pointer to a buffer with the color green data
				blueData.data(),                       //!< pointer to a buffer with the color blue data
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				rowIndex.data(),                       //!< pointer to a buffer with the rowIndex
				columnIndex.data()                     //!< pointer to a buffer with the columnIndex
			);


			double r00, r10, r20;
			double r01, r11, r21;
			double r02, r12, r22;
			r00 = 1. - 2. * yRot * yRot - 2. * zRot * zRot;
			r10 = 2. *  xRot * yRot - 2. * zRot *  wRot;
			r20 = 2. *  xRot *  zRot + 2. *  yRot *  wRot;

			r01 = 2. *  xRot * yRot + 2. *  zRot *  wRot;
			r11 = 1. - 2. *  xRot *  xRot - 2. *  zRot *  zRot;
			r21 = 2. *  yRot *  zRot - 2. *  xRot *  wRot;

			r02 = 2. *  xRot *  zRot - 2. *  yRot *  wRot;
			r12 = 2. *  yRot *  zRot + 2. *  xRot *  wRot;
			r22 = 1. - 2. *  xRot *  xRot - 2. *  yRot *  yRot;


			unsigned size = 0;
			size_t col = 0;
			size_t  row = 0;
			int64_t count = 0;
			mypt3d center(xTrans, yTrans, zTrans, 0, 0, 0, 0);
			while (size = dataReader.read()) {
				std::vector<mypt3d> pts;
				for (int64_t i = 0; i < size; ++i) {
					if (columnIndex.data())
						col = columnIndex[i];
					else
						col = 0;        //point cloud case

					if (rowIndex.data())
						row = rowIndex[i];
					else
						row = count;    //point cloud case

					mypt3d pt;
					pt.x = xData[i] * r00 + yData[i] * r10 + zData[i] * r20 + xTrans;
					pt.y = xData[i] * r01 + yData[i] * r11 + zData[i] * r21 + yTrans;
					pt.z = xData[i] * r02 + yData[i] * r12 + zData[i] * r22 + zTrans;

					if (bIntens) {         //Normalize intensity to 0 - 1.
						double intensity = (intData[i] - intOffset) / intRange;
						pt.intensity = intensity;
					}
					else {
						pt.intensity = 0.5;
					}

					if (bColor) {                     //Normalize color to 0 - 255
						int red = (int)(((redData[i] - colorRedOffset) * 255) / colorRedRange);
						int green = (int)(((greenData[i] - colorGreenOffset) * 255) / colorGreenRange);
						int blue = (int)(((blueData[i] - colorBlueOffset) * 255) / colorBlueRange);
						pt.r = red;
						pt.g = green;
						pt.b = blue;
					}
					else {
						pt.r = pt.intensity * 255;
						pt.g = pt.intensity * 255;
						pt.b = pt.intensity * 255;
					}
					pts.push_back(pt);
					count++;
					nPointsCpt++;
				}
				addVector(pts);
				if (nPointsCpt >= bufferLimit || nPointsCpt > nPoints -5) {
					writeInFiles();
					nPointsCpt = 0;
				}
			}
		}
	}
}
void CutIntoParts::readPTS() {
	unsigned sizeBuff = 1024 * 1024 * 64;
	std::vector<mypt3d> ptsVec;
	ptsVec.resize(sizeBuff);
	int cpt = 0;

	std::ifstream file(m_name, std::ios::in);

	std::string line; //Nombre de points
	getline(file, line);
	while (getline(file, line)) {
		std::istringstream ss(line);
		mypt3d pt;
		ss >> pt.x >> pt.y >> pt.z >> pt.intensity >> pt.r >> pt.g >> pt.b;

		ptsVec[cpt] = pt;
		cpt++;
		if (cpt == sizeBuff - 1) {
			cpt = 0;
			addVector(ptsVec);
			writeInFiles();
		}
	}
	ptsVec.resize(cpt);
	addVector(ptsVec);
	writeInFiles();
	file.close();
}
void CutIntoParts::readLasLaz() {
	LASpoint zero;
	std::vector<mypt3d> ptsVec;

	unsigned sizeBuff = 1024 * 1024 * 64;/*Magic value, buffering 67'108'864 rows*/
	ptsVec.resize(sizeBuff);
	int cpt = 0;
	LASreadOpener lasreadopener;
	lasreadopener.set_file_name(m_name.c_str());

	LASreader* lasreader = lasreadopener.open();
	while (lasreader->read_point()) {

		LASpoint& laspt = lasreader->point;
		/*[0, 65'536] (U16) -> [0, 1]*/
		mypt3d pt(laspt.get_x(), laspt.get_y(), laspt.get_z(), (float)laspt.get_I() / 65535.0f, laspt.rgb[0], laspt.rgb[1], laspt.rgb[1]);

		ptsVec[cpt] = pt; //faire un vector
		cpt++;

		if (cpt == sizeBuff - 1) {
			cpt = 0;
			addVector(ptsVec);
			writeInFiles();
		}

	}
	ptsVec.resize(cpt);
	addVector(ptsVec);
	writeInFiles();
	lasreader->close();
}

void CutIntoParts::writeInFiles() {
	for (int i = 0; i < parts.size(); ++i) {
		parts[i].writeInFile();
	}
}

void CutIntoParts::update() {
	boost::filesystem::path p(m_name);
	//std::string nameTemp = p.stem().string() + "-temp";
	for (auto& part : parts) {
		part.update(tempNameFolder);
	}
}
void CutIntoParts::saveFiles()
{
	boost::filesystem::path p(m_name);
	//std::string nameCutter = p.stem().string() + "-cutter";
	//std::string nameTemp = p.stem().string() + "-temp";
	for (int i = 0; i < parts.size(); ++i) {
		parts[i].savePart(nameCutter, tempNameFolder, m_retExt);
	}
}

float dotProduct(mypt3d& u, cv::Vec3f& v) {
	return (u.x * v[0] + u.y * v[1] + u.z * v[2]);
}


void CutIntoParts::addOnePoint(mypt3d& pt) {
	mypt3d tmpPt = pt;
	tmpPt.x -= offset.x;
	tmpPt.y -= offset.y;
	tmpPt.z -= offset.z;

	int xIdx, yIdx, zIdx;
	float X = dotProduct(tmpPt, m_xAxis) / m_xAxis.dot(m_xAxis);
	float Y = dotProduct(tmpPt, m_yAxis) / m_yAxis.dot(m_yAxis);
	float Z = dotProduct(tmpPt, m_zAxis) / m_zAxis.dot(m_zAxis);

	X = X / m_xSize;
	Y = Y / m_ySize;
	Z = Z / m_zSize;
	xIdx = (int)X;
	yIdx = (int)Y;
	zIdx = (int)Z;

	getPart(xIdx, yIdx, zIdx).addPoint(pt);

}

void CutIntoParts::addOnePointCutFile(mypt3d& pt) {
	//si on considère que le point d'origine est a (0,0,0)
	//et que les axes sont (0,0,0).

	//On va chercher dans quel X Y et Z se trouve le point présent. (avec l'overlap)
	for (int i = 0; i < parts.size(); ++i) {
		if (parts[i].isIn(pt ,m_overlap)) {
			parts[i].addPoint(pt);
		}
	}
}

void CutIntoParts::addVector(std::vector<mypt3d>& pts) {
	if (!cutFileIncluded) {
		for (int i = 0; i < pts.size(); ++i) {
			addOnePoint(pts[i]);
		}
	}
	else {
		for (int i = 0; i < pts.size(); ++i) {
			addOnePointCutFile(pts[i]);
		}
	}
}

void CutIntoParts::clean() {
	//boost::filesystem::path p(m_name);
	//tempNameFolder = p.parent_path().string() + "\\"p.stem().string() + "-temp"; //for the temporary files
	boost::filesystem::remove_all(tempNameFolder);
}

