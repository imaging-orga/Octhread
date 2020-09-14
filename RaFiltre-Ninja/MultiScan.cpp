#include "MultiScan.h"

MultiScan::MultiScan(std::string _nameIn, std::string _nameOut) : m_nameIn(_nameIn), m_nameOut(_nameOut)
{
}

inline float distance2pts(const mypt3d& pt1, const mypt3d& pt2) {
	float D_x = pt2.x - pt1.x;
	float D_y = pt2.y - pt1.y;
	float D_z = pt2.z - pt1.z;

	return D_x * D_x + D_y * D_y + D_z * D_z;
}

bool MultiScan::process(PARAMS::filter_params& par, FILE_OUT_TYPE type_out) {
	std::string ext = boost::filesystem::extension(par.nameOut);
	if (ext == ".e57") {
		return processE57(par, type_out);
	}
	if (ext == ".ptx") {
		return processPTX(par, type_out);
	}
	return true;
}
bool MultiScan::processE57(PARAMS::filter_params& par, FILE_OUT_TYPE type_out) {
	if (type_out == UNIQUE_FILE) {
		//reader
		e57::Reader eReader(m_nameIn);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		int numberOfScan = eReader.GetData3DCount();

		//writer
		e57::Writer eWriter(m_nameOut, "");
		e57::Data3D scanHeader;
		scanHeader.name = m_nameOut;
		scanHeader.description = "";
		scanHeader.acquisitionStart.isAtomicClockReferenced = 0;

		//scanHeader.
		for (int i = 0; i < numberOfScan; ++i) {

			//READ FILES
			std::vector<mypt3d> pts;
			e57::Data3D scanHeader_r;
			eReader.ReadData3D(i, scanHeader_r);

			int64_t sizeChunks = 1024 * 1024 * 10;
			int64_t nColumn = 0;
			int64_t nRow = 0;

			int64_t nPoints = 0;        //Number of points

			int64_t nGroupsSize = 0;        //Number of groups
			int64_t nCountSize = 0;         //Number of points per group
			bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

			eReader.GetData3DSizes(i, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
			int64_t nSize = nRow;

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

			if (scanHeader_r.pointFields.cartesianInvalidStateField) {
				isInvalidData.resize(sizeChunks);
				bInvalidState = true;
			}



			std::vector<double> xData, yData, zData;
			if (scanHeader_r.pointFields.cartesianXField)
				xData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianYField)
				yData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianZField)
				zData.resize(sizeChunks);


			bool bIntens = false;
			std::vector<double>intData;
			double intRange = 0;
			double intOffset = 0;
			if (scanHeader_r.pointFields.intensityField) {
				bIntens = true;
				intData.resize(sizeChunks);
				intRange = scanHeader_r.intensityLimits.intensityMaximum - scanHeader_r.intensityLimits.intensityMinimum;
				intOffset = scanHeader_r.intensityLimits.intensityMinimum;
			}

			bool bColor = false;
			std::vector<uint16_t> redData, greenData, blueData;
			double colorRedRange = 1;
			double colorRedOffset = 0;
			double colorGreenRange = 1;
			double colorGreenOffset = 0;
			double colorBlueRange = 1;
			double colorBlueOffset = 0;
			bColor = scanHeader_r.pointFields.colorRedField;
			
			redData.resize(sizeChunks);
			greenData.resize(sizeChunks);
			blueData.resize(sizeChunks);
			colorRedRange = scanHeader_r.colorLimits.colorRedMaximum - scanHeader_r.colorLimits.colorRedMinimum;
			colorRedOffset = scanHeader_r.colorLimits.colorRedMinimum;
			colorGreenRange = scanHeader_r.colorLimits.colorGreenMaximum - scanHeader_r.colorLimits.colorGreenMinimum;
			colorGreenOffset = scanHeader_r.colorLimits.colorGreenMinimum;
			colorBlueRange = scanHeader_r.colorLimits.colorBlueMaximum - scanHeader_r.colorLimits.colorBlueMinimum;
			colorBlueOffset = scanHeader_r.colorLimits.colorBlueMinimum;
		

			std::vector<int64_t> idElementValue;
			std::vector<int64_t> startPointIndex;
			std::vector<int64_t> pointCount;
			if (nGroupsSize > 0)
			{
				idElementValue.resize(nGroupsSize);
				startPointIndex.resize(nGroupsSize);
				pointCount.resize(nGroupsSize);

				if (!eReader.ReadData3DGroupsData(i, nGroupsSize, idElementValue.data(),
					startPointIndex.data(), pointCount.data()))
					nGroupsSize = 0;
			}



			std::vector<int32_t> rowIndex;
			std::vector<int32_t> columnIndex;
			if (scanHeader_r.pointFields.rowIndexField)
				rowIndex.resize(sizeChunks);
			if (scanHeader_r.pointFields.columnIndexField)
				columnIndex.resize(sizeChunks);
			e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
				i,                      //!< data block index given by the NewData3D
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
			unsigned size = 0;
			size_t col = 0;
			size_t  row = 0;
			int64_t count = 0;

			double xTrans = scanHeader_r.pose.translation.x;
			double yTrans = scanHeader_r.pose.translation.y;
			double zTrans = scanHeader_r.pose.translation.z;
			mypt3d center(xTrans, yTrans, zTrans, 0, 0, 0, 0);
			float squaredDist = par.distance_max * par.distance_max;
			if (par.distance_max != 0.) {
				while (size = dataReader.read()) {
					for (int64_t i = 0; i < size; ++i) {
						if (columnIndex.data())
							col = columnIndex[i];
						else
							col = 0;

						if (rowIndex.data())
							row = rowIndex[i];
						else
							row = count;

						mypt3d pt;
						pt.x = xData[i];
						pt.y = yData[i];
						pt.z = zData[i];

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

				}
			}
			else { //Distmax = 0 donc on s'occupe pas de la distance du tout. Pas besoin de rajouter un if supplémentaire
				while (size = dataReader.read()) {
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
						pt.x = xData[i];
						pt.y = yData[i];
						pt.z = zData[i];

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

				}
				std::cout << " in : " << scanHeader_r.name << std::endl;
			}

			//FILTERING:
			F::filterParticularVector(pts, par);


			//WRITE
			if (pts.size() != 0) {
				long long int new_nRow = pts.size();
				e57::Data3D scanHeader_w;
				scanHeader_w.name = scanHeader_r.name;
				std::cout << " out : " << scanHeader_w.name << std::endl;

				scanHeader_w.guid = scanHeader_r.guid;
				scanHeader_w.indexBounds.rowMaximum = new_nRow - 1;
				scanHeader_w.indexBounds.rowMinimum = 0;
				scanHeader_w.indexBounds.columnMaximum = 0;
				scanHeader_w.indexBounds.columnMinimum = 0;
				scanHeader_w.indexBounds.returnMaximum = 0;
				scanHeader_w.indexBounds.returnMinimum = 0;

				scanHeader_w.pointGroupingSchemes.groupingByLine.groupsSize = 1;
				scanHeader_w.pointGroupingSchemes.groupingByLine.pointCountSize = new_nRow;
				scanHeader_w.pointGroupingSchemes.groupingByLine.idElementName = "columnIndex";

				scanHeader_w.pointsSize = new_nRow;

				scanHeader_w.pose.rotation.w = scanHeader_r.pose.rotation.w;
				scanHeader_w.pose.rotation.x = scanHeader_r.pose.rotation.x;
				scanHeader_w.pose.rotation.y = scanHeader_r.pose.rotation.y;
				scanHeader_w.pose.rotation.z = scanHeader_r.pose.rotation.z;
				scanHeader_w.pose.translation.x = scanHeader_r.pose.translation.x;
				scanHeader_w.pose.translation.y = scanHeader_r.pose.translation.y;
				scanHeader_w.pose.translation.z = scanHeader_r.pose.translation.z;

				scanHeader_w.pointFields.cartesianXField = true;
				scanHeader_w.pointFields.cartesianYField = true;
				scanHeader_w.pointFields.cartesianZField = true;

				std::vector<double> xData_w(new_nRow);
				std::vector<double> yData_w(new_nRow);
				std::vector<double> zData_w(new_nRow);

				scanHeader_w.pointFields.cartesianInvalidStateField = true;
				std::vector<int8_t> _isInvalidData_w(new_nRow);

				std::vector<uint16_t> redData_w;
				std::vector<uint16_t> greenData_w;
				std::vector<uint16_t> blueData_w;

				scanHeader_w.pointFields.colorRedField = true;
				scanHeader_w.pointFields.colorGreenField = true;
				scanHeader_w.pointFields.colorBlueField = true;

				redData_w.resize(new_nRow);
				blueData_w.resize(new_nRow);
				greenData_w.resize(new_nRow);

				scanHeader_w.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
				scanHeader_w.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
				scanHeader_w.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
				scanHeader_w.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
				scanHeader_w.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
				scanHeader_w.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;

				std::vector<double> intData_w;
				if (bIntens) {
					scanHeader_w.pointFields.intensityField = true;

					intData_w.resize(new_nRow);

					scanHeader_w.intensityLimits.intensityMinimum = 0.;
					scanHeader_w.intensityLimits.intensityMaximum = 1.;
					scanHeader_w.pointFields.intensityScaledInteger = 0.;
				}

				int scanIndex = eWriter.NewData3D(scanHeader_w);

				e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
					scanIndex,
					new_nRow,
					xData_w.data(), yData_w.data(), zData_w.data(),
					_isInvalidData_w.data(),
					intData_w.data(),
					NULL,
					redData_w.data(), greenData_w.data(), blueData_w.data()
				);
				std::vector<int64_t> idElementValue_w;
				std::vector<int64_t> startPointIndex_w;
				vector<int64_t> pointCount_w;
				int group = 0;
				int startPoint = 0;


				int count_w = 0;
				for (long i = 0; i < new_nRow; i++) {
					//_isInvalidData[count] = isInvalidData[count];
					size_t index = i;
					mypt3d& p = pts[index];

					xData_w[count_w] = p.x;
					yData_w[count_w] = p.y;
					zData_w[count_w] = p.z;

					if (bIntens)
						intData_w[count_w] = p.intensity;

					redData_w[count_w] = p.r;
					greenData_w[count_w] = p.g;
					blueData_w[count_w] = p.b;
					count_w++;
				}

				dataWriter.write(count_w);

				idElementValue_w.push_back(0);
				startPointIndex_w.push_back(startPoint);
				pointCount_w.push_back(count_w);

				group++;
				startPoint += count;
				/*}*/
				dataWriter.close();
				eWriter.WriteData3DGroupsData(scanIndex, group,
					&idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);
			}
			else
			{
				std::cout << "fichier Vide" << std::endl;
			}

		}
		eReader.Close();
		eWriter.Close();

	}

	else {//TYPE_OUT == MULTIPLE_FILE
		std::string new_name = m_nameOut + "-mutliple";
		CreateDirectory(new_name.c_str(), NULL);
		e57::Reader eReader(m_nameIn);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		int numberOfScan = eReader.GetData3DCount();
		for (int i = 0; i < numberOfScan; ++i) {
			//writer
			e57::Data3D info;
			eReader.ReadData3D(i, info);


			//READ FILES
			std::vector<mypt3d> pts;
			e57::Data3D scanHeader_r;
			eReader.ReadData3D(i, scanHeader_r);
			std::string ext = boost::filesystem::extension(m_nameOut);
			std::string name = new_name + "\\" + scanHeader_r.name + ".e57";
			e57::Writer eWriter(name, "");
			e57::Data3D scanHeader;
			scanHeader.name = scanHeader_r.name;
			scanHeader.description = "";
			int64_t sizeChunks = 1024 * 1024 * 10;
			int64_t nColumn = 0;
			int64_t nRow = 0;

			int64_t nPoints = 0;        //Number of points

			int64_t nGroupsSize = 0;        //Number of groups
			int64_t nCountSize = 0;         //Number of points per group
			bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

			eReader.GetData3DSizes(i, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
			int64_t nSize = nRow;

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

			if (scanHeader_r.pointFields.cartesianInvalidStateField) {
				isInvalidData.resize(sizeChunks);
				bInvalidState = true;
			}



			std::vector<double> xData, yData, zData;
			if (scanHeader_r.pointFields.cartesianXField)
				xData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianYField)
				yData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianZField)
				zData.resize(sizeChunks);


			bool bIntens = false;
			std::vector<double>intData;
			double intRange = 0;
			double intOffset = 0;
			if (scanHeader_r.pointFields.intensityField) {
				bIntens = true;
				intData.resize(sizeChunks);
				intRange = scanHeader_r.intensityLimits.intensityMaximum - scanHeader_r.intensityLimits.intensityMinimum;
				intOffset = scanHeader_r.intensityLimits.intensityMinimum;
			}

			bool bColor = false;
			std::vector<uint16_t> redData, greenData, blueData;
			double colorRedRange = 1;
			double colorRedOffset = 0;
			double colorGreenRange = 1;
			double colorGreenOffset = 0;
			double colorBlueRange = 1;
			double colorBlueOffset = 0;
			bColor = scanHeader_r.pointFields.colorRedField;
			//bColor = true;
			redData.resize(sizeChunks);
			greenData.resize(sizeChunks);
			blueData.resize(sizeChunks);
			colorRedRange = scanHeader_r.colorLimits.colorRedMaximum - scanHeader_r.colorLimits.colorRedMinimum;
			colorRedOffset = scanHeader_r.colorLimits.colorRedMinimum;
			colorGreenRange = scanHeader_r.colorLimits.colorGreenMaximum - scanHeader_r.colorLimits.colorGreenMinimum;
			colorGreenOffset = scanHeader_r.colorLimits.colorGreenMinimum;
			colorBlueRange = scanHeader_r.colorLimits.colorBlueMaximum - scanHeader_r.colorLimits.colorBlueMinimum;
			colorBlueOffset = scanHeader_r.colorLimits.colorBlueMinimum;
		

			std::vector<int64_t> idElementValue;
			std::vector<int64_t> startPointIndex;
			std::vector<int64_t> pointCount;
			if (nGroupsSize > 0)
			{
				idElementValue.resize(nGroupsSize);
				startPointIndex.resize(nGroupsSize);
				pointCount.resize(nGroupsSize);

				if (!eReader.ReadData3DGroupsData(i, nGroupsSize, idElementValue.data(),
					startPointIndex.data(), pointCount.data()))
					nGroupsSize = 0;
			}



			std::vector<int32_t> rowIndex;
			std::vector<int32_t> columnIndex;
			if (scanHeader_r.pointFields.rowIndexField)
				rowIndex.resize(sizeChunks);
			if (scanHeader_r.pointFields.columnIndexField)
				columnIndex.resize(sizeChunks);
			e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
				i,                      //!< data block index given by the NewData3D
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
			unsigned size = 0;
			size_t col = 0;
			size_t  row = 0;
			int64_t count = 0;

			double xTrans = scanHeader_r.pose.translation.x;
			double yTrans = scanHeader_r.pose.translation.y;
			double zTrans = scanHeader_r.pose.translation.z;
			mypt3d center(0, 0, 0, 0, 0, 0, 0);
			float squaredDist = par.distance_max * par.distance_max;
			if (par.distance_max != 0.) {
				while (size = dataReader.read()) {
					for (int64_t i = 0; i < size; ++i) {
						if (columnIndex.data())
							col = columnIndex[i];
						else
							col = 0;

						if (rowIndex.data())
							row = rowIndex[i];
						else
							row = count;

						mypt3d pt;
						pt.x = xData[i];
						pt.y = yData[i];
						pt.z = zData[i];

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

				}
			}
			else { //Distmax = 0 donc on s'occupe pas de la distance du tout. Pas besoin de rajouter un if supplémentaire
				while (size = dataReader.read()) {
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
						pt.x = xData[i];
						pt.y = yData[i];
						pt.z = zData[i];

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

				}
				std::cout << " in : " << scanHeader_r.name << std::endl;
			}

			//FILTERING:
			F::filterParticularVector(pts, par);


			//WRITE
			if (pts.size() != 0) {
				long long int new_nRow = pts.size();
				e57::Data3D scanHeader_w;
				scanHeader_w.name = scanHeader_r.name;

				std::cout << " out : " << scanHeader_w.name << std::endl;

				scanHeader_w.guid = scanHeader_r.guid;
				scanHeader_w.indexBounds.rowMaximum = new_nRow - 1;
				scanHeader_w.indexBounds.rowMinimum = 0;
				scanHeader_w.indexBounds.columnMaximum = 0;
				scanHeader_w.indexBounds.columnMinimum = 0;
				scanHeader_w.indexBounds.returnMaximum = 0;
				scanHeader_w.indexBounds.returnMinimum = 0;

				scanHeader_w.pointGroupingSchemes.groupingByLine.groupsSize = 1;
				scanHeader_w.pointGroupingSchemes.groupingByLine.pointCountSize = new_nRow;
				scanHeader_w.pointGroupingSchemes.groupingByLine.idElementName = "columnIndex";

				scanHeader_w.pointsSize = new_nRow;

				scanHeader_w.pose.rotation.w = scanHeader_r.pose.rotation.w;
				scanHeader_w.pose.rotation.x = scanHeader_r.pose.rotation.x;
				scanHeader_w.pose.rotation.y = scanHeader_r.pose.rotation.y;
				scanHeader_w.pose.rotation.z = scanHeader_r.pose.rotation.z;
				scanHeader_w.pose.translation.x = scanHeader_r.pose.translation.x;
				scanHeader_w.pose.translation.y = scanHeader_r.pose.translation.y;
				scanHeader_w.pose.translation.z = scanHeader_r.pose.translation.z;

				scanHeader_w.pointFields.cartesianXField = true;
				scanHeader_w.pointFields.cartesianYField = true;
				scanHeader_w.pointFields.cartesianZField = true;

				std::vector<double> xData_w(new_nRow);
				std::vector<double> yData_w(new_nRow);
				std::vector<double> zData_w(new_nRow);

				scanHeader_w.pointFields.cartesianInvalidStateField = true;
				std::vector<int8_t> _isInvalidData_w(new_nRow);

				std::vector<uint16_t> redData_w;
				std::vector<uint16_t> greenData_w;
				std::vector<uint16_t> blueData_w;

				scanHeader_w.pointFields.colorRedField = true;
				scanHeader_w.pointFields.colorGreenField = true;
				scanHeader_w.pointFields.colorBlueField = true;

				redData_w.resize(new_nRow);
				blueData_w.resize(new_nRow);
				greenData_w.resize(new_nRow);

				scanHeader_w.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
				scanHeader_w.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
				scanHeader_w.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
				scanHeader_w.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
				scanHeader_w.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
				scanHeader_w.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;
				

				std::vector<double> intData_w;
				if (bIntens) {
					scanHeader_w.pointFields.intensityField = true;

					intData_w.resize(new_nRow);

					scanHeader_w.intensityLimits.intensityMinimum = 0.;
					scanHeader_w.intensityLimits.intensityMaximum = 1.;
					scanHeader_w.pointFields.intensityScaledInteger = 0.;
				}

				int scanIndex = eWriter.NewData3D(scanHeader_w);

				e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
					scanIndex,
					new_nRow,
					xData_w.data(), yData_w.data(), zData_w.data(),
					_isInvalidData_w.data(),
					intData_w.data(),
					NULL,
					redData_w.data(), greenData_w.data(), blueData_w.data()
				);
				std::vector<int64_t> idElementValue_w;
				std::vector<int64_t> startPointIndex_w;
				vector<int64_t> pointCount_w;
				int group = 0;


				int count_w = 0;
				for (long i = 0; i < new_nRow; i++) {
					//_isInvalidData[count] = isInvalidData[count];
					size_t index = i;
					mypt3d& p = pts[index];

					xData_w[count_w] = p.x;
					yData_w[count_w] = p.y;
					zData_w[count_w] = p.z;

					if (bIntens)
						intData_w[count_w] = p.intensity;

					redData_w[count_w] = p.r;
					greenData_w[count_w] = p.g;
					blueData_w[count_w] = p.b;
					count_w++;
				}

				dataWriter.write(count_w);

				idElementValue_w.push_back(0);
				startPointIndex_w.push_back(0);
				pointCount_w.push_back(count_w);

				group++;
				/*}*/
				dataWriter.close();
				eWriter.WriteData3DGroupsData(scanIndex, group,
					&idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);
				eWriter.Close();
			}
			else
			{
				std::cout << "fichier Vide" << std::endl;
			}
		}
		eReader.Close();
	}

	return true;
}

bool MultiScan::processPTX(PARAMS::filter_params& par, FILE_OUT_TYPE type_out) {
	if (type_out == UNIQUE_FILE) {
		//reader
		e57::Reader eReader(m_nameIn);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		int numberOfScan = eReader.GetData3DCount();

		std::ofstream offFile(par.nameOut, std::ios::out | std::ios::trunc);


		//writer
		e57::Data3D scanHeader;
		scanHeader.name = m_nameOut;
		scanHeader.description = "";
		scanHeader.acquisitionStart.isAtomicClockReferenced = 1;

		//scanHeader.
		for (int i = 0; i < numberOfScan; ++i) {

			//READ FILES
			std::vector<mypt3d> pts;
			e57::Data3D scanHeader_r;
			eReader.ReadData3D(i, scanHeader_r);

			int64_t sizeChunks = 1024 * 1024 * 10;
			int64_t nColumn = 0;
			int64_t nRow = 0;

			int64_t nPoints = 0;        //Number of points

			int64_t nGroupsSize = 0;        //Number of groups
			int64_t nCountSize = 0;         //Number of points per group
			bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

			eReader.GetData3DSizes(i, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
			int64_t nSize = nRow;

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

			if (scanHeader_r.pointFields.cartesianInvalidStateField) {
				isInvalidData.resize(sizeChunks);
				bInvalidState = true;
			}



			std::vector<double> xData, yData, zData;
			if (scanHeader_r.pointFields.cartesianXField)
				xData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianYField)
				yData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianZField)
				zData.resize(sizeChunks);


			bool bIntens = false;
			std::vector<double>intData;
			double intRange = 0;
			double intOffset = 0;
			if (scanHeader_r.pointFields.intensityField) {
				bIntens = true;
				intData.resize(sizeChunks);
				intRange = scanHeader_r.intensityLimits.intensityMaximum - scanHeader_r.intensityLimits.intensityMinimum;
				intOffset = scanHeader_r.intensityLimits.intensityMinimum;
			}

			bool bColor = true;
			std::vector<uint16_t> redData, greenData, blueData;
			double colorRedRange = 1;
			double colorRedOffset = 0;
			double colorGreenRange = 1;
			double colorGreenOffset = 0;
			double colorBlueRange = 1;
			double colorBlueOffset = 0;
				bColor = true;
				redData.resize(sizeChunks);
				greenData.resize(sizeChunks);
				blueData.resize(sizeChunks);
				colorRedRange = scanHeader_r.colorLimits.colorRedMaximum - scanHeader_r.colorLimits.colorRedMinimum;
				colorRedOffset = scanHeader_r.colorLimits.colorRedMinimum;
				colorGreenRange = scanHeader_r.colorLimits.colorGreenMaximum - scanHeader_r.colorLimits.colorGreenMinimum;
				colorGreenOffset = scanHeader_r.colorLimits.colorGreenMinimum;
				colorBlueRange = scanHeader_r.colorLimits.colorBlueMaximum - scanHeader_r.colorLimits.colorBlueMinimum;
				colorBlueOffset = scanHeader_r.colorLimits.colorBlueMinimum;

			std::vector<int64_t> idElementValue;
			std::vector<int64_t> startPointIndex;
			std::vector<int64_t> pointCount;
			if (nGroupsSize > 0)
			{
				idElementValue.resize(nGroupsSize);
				startPointIndex.resize(nGroupsSize);
				pointCount.resize(nGroupsSize);

				if (!eReader.ReadData3DGroupsData(i, nGroupsSize, idElementValue.data(),
					startPointIndex.data(), pointCount.data()))
					nGroupsSize = 0;
			}



			std::vector<int32_t> rowIndex;
			std::vector<int32_t> columnIndex;
			if (scanHeader_r.pointFields.rowIndexField)
				rowIndex.resize(sizeChunks);
			if (scanHeader_r.pointFields.columnIndexField)
				columnIndex.resize(sizeChunks);
			e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
				i,                      //!< data block index given by the NewData3D
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
			unsigned size = 0;
			size_t col = 0;
			size_t  row = 0;
			int64_t count = 0;
/*
			double xTrans = scanHeader_r.pose.translation.x;
			double yTrans = scanHeader_r.pose.translation.y;
			double zTrans = scanHeader_r.pose.translation.z;*/
			mypt3d center(0, 0, 0, 0, 0, 0, 0);
			float squaredDist = par.distance_max * par.distance_max;

			double xTrans = scanHeader_r.pose.translation.x;
			double yTrans = scanHeader_r.pose.translation.y;
			double zTrans = scanHeader_r.pose.translation.z;

			double wRot = scanHeader_r.pose.rotation.w;
			double xRot = scanHeader_r.pose.rotation.x;
			double yRot = scanHeader_r.pose.rotation.y;
			double zRot = scanHeader_r.pose.rotation.z;
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
			if (par.distance_max != 0.) {
				while (size = dataReader.read()) {
					for (int64_t i = 0; i < size; ++i) {
						if (columnIndex.data())
							col = columnIndex[i];
						else
							col = 0;

						if (rowIndex.data())
							row = rowIndex[i];
						else
							row = count;

						mypt3d pt;
						pt.x = xData[i];
						pt.y = yData[i];
						pt.z = zData[i];

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
							pt.r = pt.intensity * 255;
							pt.g = pt.intensity * 255;
							pt.b = pt.intensity * 255;
						}
						pts.push_back(pt);
						count++;
					}

				}
			}
			else { //Distmax = 0 donc on s'occupe pas de la distance du tout. Pas besoin de rajouter un if supplémentaire
				while (size = dataReader.read()) {
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
							pt.r = color * 255;
							pt.g = color * 255;
							pt.b = color * 255;
						}

						if (pt.r == 0 && pt.g == 0 && pt.b == 0) {
							unsigned color = (unsigned)(pt.intensity * 255);
							pt.r = color * 255;
							pt.g = color * 255;
							pt.b = color * 255;
						}
						pts.push_back(pt);
						count++;
					}

				}
				std::cout << " in : " << scanHeader_r.name << std::endl;
			}

			//FILTERING:
			F::filterParticularVector(pts, par);

			//WRITE
			if (pts.size() != 0) {
			
				float x, y, z, w;
				x = scanHeader_r.pose.rotation.x;
				y = scanHeader_r.pose.rotation.y;
				z = scanHeader_r.pose.rotation.z;
				w = scanHeader_r.pose.rotation.w;

				offFile << pts.size() << "\n";
				offFile << "1" << "\n";
				//offFile <<  scanHeader_r.pose.translation.x << " " << scanHeader_r.pose.translation.y << " " << scanHeader_r.pose.translation.z << "\n";
				
				offFile << "0 0 0 \n";
				offFile << "1 0 0" << "\n";
				offFile << "0 1 0" << "\n";
				offFile << "0 0 1" << "\n";

				offFile << "1 0 0 0" << "\n";
				offFile << "0 1 0 0" << "\n";
				offFile << "0 0 1 0" << "\n";
				offFile << "0 0 0 1" << "\n";
				for (int i = 0; i < pts.size(); ++i) {
					offFile << pts[i].x << " " << pts[i].y << " " << pts[i].z << " " << pts[i].intensity << " " << (unsigned)pts[i].r << " " << (unsigned)pts[i].g << " " << (unsigned)pts[i].b << "\n";
				}
			}
			else
			{
				std::cout << "fichier Vide" << std::endl;
			}

		}
		eReader.Close();
		offFile.close();
	}

	else {//TYPE_OUT == MULTIPLE_FILE
		std::string new_name = m_nameOut + "-mutliple";
		CreateDirectory(new_name.c_str(), NULL);
		e57::Reader eReader(m_nameIn);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		int numberOfScan = eReader.GetData3DCount();
		for (int i = 0; i < numberOfScan; ++i) {
			//writer
			e57::Data3D info;
			eReader.ReadData3D(i, info);


			//READ FILES
			std::vector<mypt3d> pts;
			e57::Data3D scanHeader_r;
			eReader.ReadData3D(i, scanHeader_r);
			std::string ext = boost::filesystem::extension(m_nameOut);
			std::string name = new_name + "\\" + scanHeader_r.name + ext;
		
			e57::Data3D scanHeader;
			scanHeader.name = scanHeader_r.name;
			scanHeader.description = "";
			int64_t sizeChunks = 1024 * 1024 * 10;
			int64_t nColumn = 0;
			int64_t nRow = 0;

			int64_t nPoints = 0;        //Number of points

			int64_t nGroupsSize = 0;        //Number of groups
			int64_t nCountSize = 0;         //Number of points per group
			bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

			eReader.GetData3DSizes(i, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
			int64_t nSize = nRow;

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

			if (scanHeader_r.pointFields.cartesianInvalidStateField) {
				isInvalidData.resize(sizeChunks);
				bInvalidState = true;
			}



			std::vector<double> xData, yData, zData;
			if (scanHeader_r.pointFields.cartesianXField)
				xData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianYField)
				yData.resize(sizeChunks);

			if (scanHeader_r.pointFields.cartesianZField)
				zData.resize(sizeChunks);


			bool bIntens = false;
			std::vector<double>intData;
			double intRange = 0;
			double intOffset = 0;
			if (scanHeader_r.pointFields.intensityField) {
				bIntens = true;
				intData.resize(sizeChunks);
				intRange = scanHeader_r.intensityLimits.intensityMaximum - scanHeader_r.intensityLimits.intensityMinimum;
				intOffset = scanHeader_r.intensityLimits.intensityMinimum;
			}

			std::vector<uint16_t> redData, greenData, blueData;
			double colorRedRange = 1;
			double colorRedOffset = 0;
			double colorGreenRange = 1;
			double colorGreenOffset = 0;
			double colorBlueRange = 1;
			double colorBlueOffset = 0;
			
				bool bColor = true;
				redData.resize(sizeChunks);
				greenData.resize(sizeChunks);
				blueData.resize(sizeChunks);
				colorRedRange = scanHeader_r.colorLimits.colorRedMaximum - scanHeader_r.colorLimits.colorRedMinimum;
				colorRedOffset = scanHeader_r.colorLimits.colorRedMinimum;
				colorGreenRange = scanHeader_r.colorLimits.colorGreenMaximum - scanHeader_r.colorLimits.colorGreenMinimum;
				colorGreenOffset = scanHeader_r.colorLimits.colorGreenMinimum;
				colorBlueRange = scanHeader_r.colorLimits.colorBlueMaximum - scanHeader_r.colorLimits.colorBlueMinimum;
				colorBlueOffset = scanHeader_r.colorLimits.colorBlueMinimum;
			

			std::vector<int64_t> idElementValue;
			std::vector<int64_t> startPointIndex;
			std::vector<int64_t> pointCount;
			if (nGroupsSize > 0)
			{
				idElementValue.resize(nGroupsSize);
				startPointIndex.resize(nGroupsSize);
				pointCount.resize(nGroupsSize);

				if (!eReader.ReadData3DGroupsData(i, nGroupsSize, idElementValue.data(),
					startPointIndex.data(), pointCount.data()))
					nGroupsSize = 0;
			}



			std::vector<int32_t> rowIndex;
			std::vector<int32_t> columnIndex;
			if (scanHeader_r.pointFields.rowIndexField)
				rowIndex.resize(sizeChunks);
			if (scanHeader_r.pointFields.columnIndexField)
				columnIndex.resize(sizeChunks);
			e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
				i,                      //!< data block index given by the NewData3D
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
			unsigned size = 0;
			size_t col = 0;
			size_t  row = 0;
			int64_t count = 0;


			double xTrans = scanHeader_r.pose.translation.x;
			double yTrans = scanHeader_r.pose.translation.y;
			double zTrans = scanHeader_r.pose.translation.z;

			double wRot = scanHeader_r.pose.rotation.w;
			double xRot = scanHeader_r.pose.rotation.x;
			double yRot = scanHeader_r.pose.rotation.y;
			double zRot = scanHeader_r.pose.rotation.z;
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
			mypt3d center(xTrans, yTrans, zTrans, 0, 0, 0, 0);
			float squaredDist = par.distance_max * par.distance_max;
			if (par.distance_max != 0.) {
				while (size = dataReader.read()) {
					for (int64_t i = 0; i < size; ++i) {
						if (columnIndex.data())
							col = columnIndex[i];
						else
							col = 0;

						if (rowIndex.data())
							row = rowIndex[i];
						else
							row = count;

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
							pt.r = color * 255;
							pt.g = color * 255;
							pt.b = color * 255;
						}
						if (pt.r == 0 && pt.g == 0 && pt.b == 0) {
							unsigned color = (unsigned)(pt.intensity * 255);
							pt.r = color * 255;
							pt.g = color * 255;
							pt.b = color * 255;
						}
						pts.push_back(pt);
						count++;
					}

				}
			}
			else { //Distmax = 0 donc on s'occupe pas de la distance du tout. Pas besoin de rajouter un if supplémentaire
				while (size = dataReader.read()) {
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
							pt.r = color * 255;
							pt.g = color * 255;
							pt.b = color * 255;
						}

						if (pt.r == 0 && pt.g == 0 && pt.b == 0) {
							unsigned color = (unsigned)(pt.intensity * 255);
							pt.r = color * 255;
							pt.g = color * 255;
							pt.b = color * 255;
						}
						pts.push_back(pt);
						count++;
					}

				}
				std::cout << " in : " << scanHeader_r.name << std::endl;
			}

			//FILTERING:
			F::filterParticularVector(pts, par);


			//WRITE
			if (pts.size() != 0) {
				std::ofstream offFile(new_name + "\\" + scanHeader_r.name + ext, std::ios::out | std::ios::trunc);


				offFile << pts.size() << "\n";
				offFile << "1" << "\n";
				offFile << "0 0 0" << "\n";
				offFile << "1 0 0" << "\n";
				offFile << "0 1 0" << "\n";
				offFile << "0 0 1" << "\n";

				offFile << "1 0 0 0 \n";
				offFile << "0 1 0 0 \n";
				offFile << "0 0 1 0 \n";
				offFile << "0 0 0 1 \n";

				for (int i = 0; i < pts.size(); ++i) {
					offFile << pts[i].x << " " << pts[i].y << " " << pts[i].z << " " << pts[i].intensity << " " << (unsigned)pts[i].r << " " << (unsigned)pts[i].g << " " << (unsigned)pts[i].b << "\n";
				}
				offFile.close();
			}
			else
			{
				std::cout << "fichier Vide" << std::endl;
			}
		}
		eReader.Close();
	}

	return true;
}



bool MultiScan::RemovePointe57() {
	//Lire le fichier d'entrée


	e57::Reader eReader(m_nameIn);
	e57::E57Root rootHeader;
	eReader.GetE57Root(rootHeader);
	std::string guid = rootHeader.guid;
	std::string coordinateMetaData = rootHeader.coordinateMetadata;
	int numberOfScan = eReader.GetData3DCount();

	//writer
	e57::Writer eWriter(m_nameOut, "");
	e57::Data3D scanHeader;
	scanHeader.name = m_nameOut;
	scanHeader.description = "";
	scanHeader.acquisitionStart.isAtomicClockReferenced = 0;

	for (int i = 0; i < numberOfScan; ++i) {

		//READ FILES
		std::vector<mypt3d> pts;
		e57::Data3D scanHeader_r;
		eReader.ReadData3D(i, scanHeader_r);

		int64_t sizeChunks = 1024 * 1024 * 10;
		int64_t nColumn = 0;
		int64_t nRow = 0;

		int64_t nPoints = 0;        //Number of points

		int64_t nGroupsSize = 0;        //Number of groups
		int64_t nCountSize = 0;         //Number of points per group
		bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

		eReader.GetData3DSizes(i, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
		int64_t nSize = nRow;

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

		if (scanHeader_r.pointFields.cartesianInvalidStateField) {
			isInvalidData.resize(sizeChunks);
			bInvalidState = true;
		}

		double xTrans = scanHeader_r.pose.translation.x;
		double yTrans = scanHeader_r.pose.translation.y;
		double zTrans = scanHeader_r.pose.translation.z;
		mypt3d center(xTrans, yTrans, zTrans, 0, 0, 0, 0);

		//ECRITURE
		long long int new_nRow = 100;
		e57::Data3D scanHeader_w;
		scanHeader_w.name = scanHeader_r.name;
		std::cout << " out : " << scanHeader_w.name << std::endl;

		scanHeader_w.guid = scanHeader_r.guid;
		scanHeader_w.indexBounds.rowMaximum = new_nRow;
		scanHeader_w.indexBounds.rowMinimum = 0;
		scanHeader_w.indexBounds.columnMaximum = 0;
		scanHeader_w.indexBounds.columnMinimum = 0;
		scanHeader_w.indexBounds.returnMaximum = 0;
		scanHeader_w.indexBounds.returnMinimum = 0;

		scanHeader_w.pointGroupingSchemes.groupingByLine.groupsSize = 1;
		scanHeader_w.pointGroupingSchemes.groupingByLine.pointCountSize = new_nRow;
		scanHeader_w.pointGroupingSchemes.groupingByLine.idElementName = "columnIndex";

		scanHeader_w.pointsSize = new_nRow;

		scanHeader_w.pose.rotation.w = scanHeader_r.pose.rotation.w;
		scanHeader_w.pose.rotation.x = scanHeader_r.pose.rotation.x;
		scanHeader_w.pose.rotation.y = scanHeader_r.pose.rotation.y;
		scanHeader_w.pose.rotation.z = scanHeader_r.pose.rotation.z;
		scanHeader_w.pose.translation.x = scanHeader_r.pose.translation.x;
		scanHeader_w.pose.translation.y = scanHeader_r.pose.translation.y;
		scanHeader_w.pose.translation.z = scanHeader_r.pose.translation.z;

		scanHeader_w.pointFields.cartesianXField = true;
		scanHeader_w.pointFields.cartesianYField = true;
		scanHeader_w.pointFields.cartesianZField = true;

		std::vector<double> xData_w(new_nRow);
		std::vector<double> yData_w(new_nRow);
		std::vector<double> zData_w(new_nRow);

		scanHeader_w.pointFields.cartesianInvalidStateField = true;
		std::vector<int8_t> _isInvalidData_w(new_nRow);

		std::vector<uint16_t> redData_w;
		std::vector<uint16_t> greenData_w;
		std::vector<uint16_t> blueData_w;

		scanHeader_w.pointFields.colorRedField = true;
		scanHeader_w.pointFields.colorGreenField = true;
		scanHeader_w.pointFields.colorBlueField = true;

		redData_w.resize(new_nRow);
		blueData_w.resize(new_nRow);
		greenData_w.resize(new_nRow);

		scanHeader_w.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
		scanHeader_w.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
		scanHeader_w.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
		scanHeader_w.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
		scanHeader_w.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
		scanHeader_w.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;

		std::vector<double> intData_w;
		scanHeader_w.pointFields.intensityField = true;

		intData_w.resize(new_nRow);

		scanHeader_w.intensityLimits.intensityMinimum = 0.;
		scanHeader_w.intensityLimits.intensityMaximum = 1.;
		scanHeader_w.pointFields.intensityScaledInteger = 0.;

		int scanIndex = eWriter.NewData3D(scanHeader_w);

		e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
			scanIndex,
			new_nRow,
			xData_w.data(), yData_w.data(), zData_w.data(),
			_isInvalidData_w.data(),
			intData_w.data(),
			NULL,
			redData_w.data(), greenData_w.data(), blueData_w.data()
		);
		std::vector<int64_t> idElementValue_w;
		std::vector<int64_t> startPointIndex_w;
		vector<int64_t> pointCount_w;
		int group = 0;
		int startPoint = 0;


		int count_w = 0;
		//_isInvalidData[count] = isInvalidData[count];
		size_t index = i;
		for (long i = 0; i < new_nRow; i++) {
			mypt3d p = mypt3d(xTrans, yTrans, zTrans, 0.5, 128, 128, 128);

			xData_w[count_w] = p.x;
			yData_w[count_w] = p.y;
			zData_w[count_w] = p.z;

			intData_w[count_w] = p.intensity;

			redData_w[count_w] = p.r;
			greenData_w[count_w] = p.g;
			blueData_w[count_w] = p.b;
			count_w++;
		}

		dataWriter.write(count_w);

		idElementValue_w.push_back(0);
		startPointIndex_w.push_back(startPoint);
		pointCount_w.push_back(count_w);

		group++;
		dataWriter.close();
		eWriter.WriteData3DGroupsData(scanIndex, group,
			&idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);
	
	}
	eReader.Close();
	eWriter.Close();
}
