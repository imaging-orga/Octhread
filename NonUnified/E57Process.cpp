//#include "E57Process.h"
//
//Non_Unifiede57::Non_Unifiede57(std::string _nameIn, std::string _nameOut) : m_nameIn(_nameIn), m_nameOut(_nameOut)
//{
//}
//
//bool Non_Unifiede57::process(PARAMS::filter_params& par) {
//	//reader
//	e57::Reader eReader(m_nameIn);
//	e57::E57Root rootHeader;
//	eReader.GetE57Root(rootHeader);
//	std::string guid = rootHeader.guid;
//	std::string coordinateMetaData = rootHeader.coordinateMetadata;
//	int numberOfScan = eReader.GetData3DCount();
//
//	//writer
//	e57::Writer eWriter(m_nameOut, "");
//	e57::Data3D scanHeader;
//	scanHeader.name = m_nameOut;
//	scanHeader.description = "";
//	
//	//scanHeader.
//	for (int i = 0; i < numberOfScan; ++i) {
//
//		std::vector<mypt3d> pts = _read(eReader, i, par.distance_max);
//		_filter(pts,par);
//		//if (_read(eReader, i))
//		//	if (_filter())
//		//		_save();
//	}
//}
//
//inline float distance2pts(const mypt3d& pt1, const mypt3d& pt2) {
//	float D_x = pt2.x - pt1.x;
//	float D_y = pt2.y - pt1.y;
//	float D_z = pt2.z - pt1.z;
//
//	return D_x * D_x + D_y * D_y + D_z * D_z;
//}
//
//
//std::vector<mypt3d> Non_Unifiede57::_read(e57::Reader& eReader, int idx, float _distMax)
//{
//	std::vector<mypt3d> ret;
//	e57::Data3D scanHeader;
//	eReader.ReadData3D(idx, scanHeader);
//	int64_t sizeChunks = 1024 * 1024 * 10;
//	int64_t nColumn = 0;
//	int64_t nRow = 0;
//
//	int64_t nPoints = 0;        //Number of points
//
//	int64_t nGroupsSize = 0;        //Number of groups
//	int64_t nCountSize = 0;         //Number of points per group
//	bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"
//
//	eReader.GetData3DSizes(idx, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
//	int64_t nSize = nRow;
//
//	if (nRow == 0 && nColumn == 0) {
//		nRow = nPoints;
//		nColumn = 1;
//
//		nGroupsSize = 1;
//		nGroupsSize = 1;
//		nCountSize = nPoints;
//		bColumnIndex = true;
//	}
//	std::vector<int8_t> isInvalidData;
//	bool bInvalidState;
//
//	if (scanHeader.pointFields.cartesianInvalidStateField) {
//		isInvalidData.resize(sizeChunks);
//		bInvalidState = true;
//	}
//
//
//
//	std::vector<double> xData, yData, zData;
//	if (scanHeader.pointFields.cartesianXField)
//		xData.resize(sizeChunks);
//
//	if (scanHeader.pointFields.cartesianYField)
//		yData.resize(sizeChunks);
//
//	if (scanHeader.pointFields.cartesianZField)
//		zData.resize(sizeChunks);
//
//
//	bool bIntens = false;
//	std::vector<double>intData;
//	double intRange = 0;
//	double intOffset = 0;
//	if (scanHeader.pointFields.intensityField) {
//		bIntens = true;
//		intData.resize(sizeChunks);
//		intRange = scanHeader.intensityLimits.intensityMaximum - scanHeader.intensityLimits.intensityMinimum;
//		intOffset = scanHeader.intensityLimits.intensityMinimum;
//	}
//
//	bool bColor = false;
//	std::vector<uint16_t> redData, greenData, blueData;
//	double colorRedRange = 1;
//	double colorRedOffset = 0;
//	double colorGreenRange = 1;
//	double colorGreenOffset = 0;
//	double colorBlueRange = 1;
//	double colorBlueOffset = 0;
//	if (scanHeader.pointFields.colorRedField)
//	{
//		bColor = true;
//		redData.resize(sizeChunks);
//		greenData.resize(sizeChunks);
//		blueData.resize(sizeChunks);
//		colorRedRange = scanHeader.colorLimits.colorRedMaximum - scanHeader.colorLimits.colorRedMinimum;
//		colorRedOffset = scanHeader.colorLimits.colorRedMinimum;
//		colorGreenRange = scanHeader.colorLimits.colorGreenMaximum - scanHeader.colorLimits.colorGreenMinimum;
//		colorGreenOffset = scanHeader.colorLimits.colorGreenMinimum;
//		colorBlueRange = scanHeader.colorLimits.colorBlueMaximum - scanHeader.colorLimits.colorBlueMinimum;
//		colorBlueOffset = scanHeader.colorLimits.colorBlueMinimum;
//	}
//
//	std::vector<int64_t> idElementValue;
//	std::vector<int64_t> startPointIndex;
//	std::vector<int64_t> pointCount;
//	if (nGroupsSize > 0)
//	{
//		idElementValue.resize(nGroupsSize);
//		startPointIndex.resize(nGroupsSize);
//		pointCount.resize(nGroupsSize);
//
//		if (!eReader.ReadData3DGroupsData(idx, nGroupsSize, idElementValue.data(),
//			startPointIndex.data(), pointCount.data()))
//			nGroupsSize = 0;
//	}
//
//
//
//	std::vector<int32_t> rowIndex;
//	std::vector<int32_t> columnIndex;
//	if (scanHeader.pointFields.rowIndexField)
//		rowIndex.resize(sizeChunks);
//	if (scanHeader.pointFields.columnIndexField)
//		columnIndex.resize(sizeChunks);
//	e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
//		idx,                      //!< data block index given by the NewData3D
//		sizeChunks,                           //!< size of each of the buffers given
//		xData.data(),                          //!< pointer to a buffer with the x data
//		yData.data(),                          //!< pointer to a buffer with the y data
//		zData.data(),                          //!< pointer to a buffer with the z data
//		isInvalidData.data(),          //!< pointer to a buffer with the valid indication
//		intData.data(),                        //!< pointer to a buffer with the lidar return intesity
//		NULL,
//		redData.data(),                        //!< pointer to a buffer with the color red data
//		greenData.data(),                      //!< pointer to a buffer with the color green data
//		blueData.data(),                       //!< pointer to a buffer with the color blue data
//		NULL,
//		NULL,
//		NULL,
//		NULL,
//		NULL,
//		rowIndex.data(),                       //!< pointer to a buffer with the rowIndex
//		columnIndex.data()                     //!< pointer to a buffer with the columnIndex
//	);
//	unsigned size = 0;
//	size_t col = 0;
//	size_t  row = 0;
//	int64_t count = 0;
//
//	double xTrans = scanHeader.pose.translation.x;
//	double yTrans = scanHeader.pose.translation.y;
//	double zTrans = scanHeader.pose.translation.z;
//	mypt3d center(xTrans, yTrans, zTrans, 0, 0, 0, 0);
//	float squaredDist = _distMax * _distMax;
//	if (_distMax != 0.) {
//		while (size = dataReader.read()) {
//			for (int i = 0; i < size; ++i) {
//				if (columnIndex.data())
//					col = columnIndex[i];
//				else
//					col = 0;
//
//				if (rowIndex.data())
//					row = rowIndex[i];
//				else
//					row = count;
//
//				mypt3d pt;
//				pt.x = xData[i];
//				pt.y = yData[i];
//				pt.z = zData[i];
//
//				if (distance2pts(center, pt) > squaredDist) {// si la distance est plus grande
//					continue;
//				}
//				if (bIntens) {         //Normalize intensity to 0 - 1.
//					double intensity;
//					if (intRange != 0)
//						intensity = (intData[i] - intOffset) / intRange;
//					else
//						intensity = intData[i];
//					pt.intensity = intensity;
//				}
//				else {
//					pt.intensity = 0.5;
//				}
//
//				if (bColor) {                     //Normalize color to 0 - 255
//					int red = (int)(((redData[i] - colorRedOffset) * 255) / colorRedRange);
//					int green = (int)(((greenData[i] - colorGreenOffset) * 255) / colorGreenRange);
//					int blue = (int)(((blueData[i] - colorBlueOffset) * 255) / colorBlueRange);
//					pt.r = red;
//					pt.g = green;
//					pt.b = blue;
//				}
//				else {
//					pt.r = pt.intensity * 255;
//					pt.g = pt.intensity * 255;
//					pt.b = pt.intensity * 255;
//				}
//				ret.push_back(pt);
//				count++;
//			}
//
//		}
//	}
//	else { //Distmax = 0 donc on s'occupe pas de la distance du tout. Pas besoin de rajouter un if supplémentaire
//		while (size = dataReader.read()) {
//			for (int64_t i = 0; i < size; ++i) {
//				if (columnIndex.data())
//					col = columnIndex[i];
//				else
//					col = 0;        //point cloud case
//
//				if (rowIndex.data())
//					row = rowIndex[i];
//				else
//					row = count;    //point cloud case
//
//				mypt3d pt;
//				pt.x = xData[i] ;
//				pt.y = xData[i] ;
//				pt.z = xData[i] ;
//
//				if (bIntens) {         //Normalize intensity to 0 - 1.
//					double intensity = (intData[i] - intOffset) / intRange;
//					pt.intensity = intensity;
//				}
//				else {
//					pt.intensity = 0.5;
//				}
//
//				if (bColor) {                     //Normalize color to 0 - 255
//					int red = (int)(((redData[i] - colorRedOffset) * 255) / colorRedRange);
//					int green = (int)(((greenData[i] - colorGreenOffset) * 255) / colorGreenRange);
//					int blue = (int)(((blueData[i] - colorBlueOffset) * 255) / colorBlueRange);
//					pt.r = red;
//					pt.g = green;
//					pt.b = blue;
//				}
//				else {
//					pt.r = pt.intensity * 255;
//					pt.g = pt.intensity * 255;
//					pt.b = pt.intensity * 255;
//				}
//				ret.push_back(pt);
//				count++;
//			}
//
//		}
//	}
//	eReader.Close();
//	return ret;
//}
//
//void Non_Unifiede57::_filter(std::vector<mypt3d>& pts, PARAMS::filter_params& par)
//{
//	F::filterParticularVector(pts, par);
//}
//
//bool Non_Unifiede57::_save(e57::Writer& eWriter, std::vector<mypt3d>& pts)
//{
//	return false;
//}
