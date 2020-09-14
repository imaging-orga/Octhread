#include "e57File.h"
#include "boost\filesystem.hpp"
#include <Windows.h>


e57File::e57File(std::string filename_, long MAXPOINTSNUMPERNODE) : OpenableFile(filename_, MAXPOINTSNUMPERNODE) {
	boost::filesystem::path p(filename_);
	CreateDirectory(p.stem().string().c_str(), NULL);


	e57::Reader eReader(p_name);
	int numberOfScan = eReader.GetData3DCount();

	e_type = (numberOfScan == 1) ? UNIFIED : NON_UNIFIED;
	BoundingBox bb = getBoundingBox();
	p_oct = Octree(p.stem().string().c_str(), bb, 0, MAXPOINTSNUMPERNODE);
}

BoundingBox e57File::getBoundingBox()
{
	BoundingBox ret;
	e57::Reader eReader(p_name);
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


void e57File::read(float distMax)
{
	if (e_type == UNIFIED)
		return read_Unified();
	else
		return read_NonUnified(distMax);
}

void e57File::read_Unified()
{
	e57::Reader eReader(p_name);
	e57::E57Root rootHeader;
	eReader.GetE57Root(rootHeader);
	std::string guid = rootHeader.guid;
	std::string coordinateMetaData = rootHeader.coordinateMetadata;
	int numberOfScan = eReader.GetData3DCount();
	e57::Data3D scanHeader;
	eReader.ReadData3D(0, scanHeader);
	int64_t sizeChunks = 1024 * 1024 * 10;

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
	bColor = scanHeader.pointFields.colorRedField;
	
	redData.resize(sizeChunks);
	greenData.resize(sizeChunks);
	blueData.resize(sizeChunks);
	colorRedRange = scanHeader.colorLimits.colorRedMaximum - scanHeader.colorLimits.colorRedMinimum;
	colorRedOffset = scanHeader.colorLimits.colorRedMinimum;
	colorGreenRange = scanHeader.colorLimits.colorGreenMaximum - scanHeader.colorLimits.colorGreenMinimum;
	colorGreenOffset = scanHeader.colorLimits.colorGreenMinimum;
	colorBlueRange = scanHeader.colorLimits.colorBlueMaximum - scanHeader.colorLimits.colorBlueMinimum;
	colorBlueOffset = scanHeader.colorLimits.colorBlueMinimum;
	

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
				unsigned color = (unsigned)(pt.intensity * 255);
				pt.r = color;
				pt.g = color;
				pt.b = color;
			}


			if (pt.r == 0 && pt.g == 0 && pt.b == 0) {
				unsigned color = (unsigned)(pt.intensity * 255);
				pt.r = color;
				pt.g = color;
				pt.b = color;
			}

			pts[i] = pt;
			count++;
		}

		p_oct.addPoint(pts);
	}
	p_oct.save();
	p_oct.clean();

}

//Renvoie la distance au carré, pour eviter de faire la racine carrée qui est une opération plutôt longue
inline float distance2pts(const mypt3d& pt1, const mypt3d& pt2)  {
	float D_x = pt2.x - pt1.x;
	float D_y = pt2.y - pt1.y;
	float D_z = pt2.z - pt1.z;

	return D_x * D_x + D_y * D_y + D_z * D_z;
}


void e57File::read_NonUnified(float distMax)
{
	e57::Reader eReader(p_name);
	e57::E57Root rootHeader;
	eReader.GetE57Root(rootHeader);
	std::string guid = rootHeader.guid;
	std::string coordinateMetaData = rootHeader.coordinateMetadata;
	int numberOfScan = eReader.GetData3DCount();
	int64_t numberOfPoints = 0;
	float squaredDist = distMax * distMax;
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
		bColor = scanHeader.pointFields.colorRedField;
		redData.resize(sizeChunks);
		greenData.resize(sizeChunks);
		blueData.resize(sizeChunks);
		colorRedRange = scanHeader.colorLimits.colorRedMaximum - scanHeader.colorLimits.colorRedMinimum;
		colorRedOffset = scanHeader.colorLimits.colorRedMinimum;
		colorGreenRange = scanHeader.colorLimits.colorGreenMaximum - scanHeader.colorLimits.colorGreenMinimum;
		colorGreenOffset = scanHeader.colorLimits.colorGreenMinimum;
		colorBlueRange = scanHeader.colorLimits.colorBlueMaximum - scanHeader.colorLimits.colorBlueMinimum;
		colorBlueOffset = scanHeader.colorLimits.colorBlueMinimum;

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
		//Faire la distinction avec une demande de filtre sur la distance ou non.
		if (distMax != 0.) {
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
					if (distance2pts(center, pt) > squaredDist) {// si la distance est plus grande

						continue;
					}

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
						unsigned color = (unsigned)(pt.intensity * 255);
						pt.r = color;
						pt.g = color;
						pt.b = color;
					}

					if (pt.r == 0 && pt.g == 0 && pt.b == 0) {
						unsigned color = (unsigned)(pt.intensity * 255);
						pt.r = color;
						pt.g = color;
						pt.b = color;
					}
					pts.push_back(pt);
					count++;
				}

				p_oct.addPoint(pts);
			}
		}
		else { //Distmax = 0 donc on s'occupe pas de la distance du tout. Pas besoin de rajouter un if supplémentaire
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
						unsigned color = (unsigned)(pt.intensity * 255);
						pt.r = color;
						pt.g = color;
						pt.b = color;
					}

					if (pt.r == 0 && pt.g == 0 && pt.b == 0) {
						unsigned color = (unsigned)(pt.intensity * 255);
						pt.r = color;
						pt.g = color;
						pt.b = color;
					}
					pts.push_back(pt);
					count++;
				}

				p_oct.addPoint(pts);
			}
		}

	}
	eReader.Close();
	p_oct.save();
	p_oct.clean();

}

std::pair<pt3d, pt3d> e57File::read_NonUnifiedForBoundingBox() {
	e57::Reader eReader(p_name);
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