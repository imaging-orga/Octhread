#include "Optimizer.h"


namespace OPTIMIZE {
	void process(std::string nameIn, std::string nameOut) {
		e57::Reader eReader(nameIn);
		int numberOfScan = eReader.GetData3DCount();
		std::cout << "Nombre de scans :: " << numberOfScan << std::endl;
		if (numberOfScan == 1) {
			std::cout << "Demarrage de UNIQUE" << std::endl;
			Unique(nameIn, nameOut);
		}
		else {
			std::cout << "Demarrage de MULTIPLE" << std::endl;
			Multiple(nameIn, nameOut);
		}
	}

	int Multiple(std::string nameIn, std::string nameOut) {
		e57::Reader eReader(nameIn);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		e57::Writer eWriter(nameOut, "");
		
		int numberOfScan = eReader.GetData3DCount();
		for (int i = 0; i < numberOfScan; ++i) {
			std::vector<MyPoint> pts;
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
			bool bInvalidState = true;

			
			isInvalidData.resize(sizeChunks);
			bInvalidState = true;



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

			
			e57::Data3D scanHeader_w;
			scanHeader_w.name = scanHeader_r.name;

			std::cout << " out : " << scanHeader_w.name << std::endl;

			scanHeader_w.guid = scanHeader_r.guid;
			scanHeader_w.indexBounds.rowMaximum = scanHeader_r.indexBounds.rowMaximum;
			scanHeader_w.indexBounds.rowMinimum = scanHeader_r.indexBounds.rowMinimum;
			scanHeader_w.indexBounds.columnMaximum = scanHeader_r.indexBounds.columnMaximum;
			scanHeader_w.indexBounds.columnMinimum = scanHeader_r.indexBounds.columnMinimum;
			scanHeader_w.indexBounds.returnMaximum = scanHeader_r.indexBounds.returnMaximum;
			scanHeader_w.indexBounds.returnMinimum = scanHeader_r.indexBounds.returnMinimum;

			scanHeader_w.pointGroupingSchemes.groupingByLine.groupsSize = scanHeader_r.pointGroupingSchemes.groupingByLine.groupsSize;
			scanHeader_w.pointGroupingSchemes.groupingByLine.pointCountSize = scanHeader_r.pointGroupingSchemes.groupingByLine.pointCountSize;
			scanHeader_w.pointGroupingSchemes.groupingByLine.idElementName = scanHeader_r.pointGroupingSchemes.groupingByLine.idElementName;

			scanHeader_w.pointsSize = scanHeader_r.pointsSize;

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

			std::vector<double> xData_w(sizeChunks);
			std::vector<double> yData_w(sizeChunks);
			std::vector<double> zData_w(sizeChunks);

			scanHeader_w.pointFields.cartesianInvalidStateField = true;
			std::vector<int8_t> _isInvalidData_w(sizeChunks);

			std::vector<uint16_t> redData_w;
			std::vector<uint16_t> greenData_w;
			std::vector<uint16_t> blueData_w;

			scanHeader_w.pointFields.colorRedField = true;
			scanHeader_w.pointFields.colorGreenField = true;
			scanHeader_w.pointFields.colorBlueField = true;

			redData_w.resize(sizeChunks);
			blueData_w.resize(sizeChunks);
			greenData_w.resize(sizeChunks);

			scanHeader_w.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
			scanHeader_w.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
			scanHeader_w.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
			scanHeader_w.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
			scanHeader_w.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
			scanHeader_w.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;


			std::vector<double> intData_w;
			scanHeader_w.pointFields.intensityField = scanHeader_r.pointFields.intensityField;

			intData_w.resize(sizeChunks);

			scanHeader_w.intensityLimits.intensityMinimum = 0.;
			scanHeader_w.intensityLimits.intensityMaximum = 1.;
			scanHeader_w.pointFields.intensityScaledInteger = 0.;


			int scanIndex = eWriter.NewData3D(scanHeader_w);


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


			e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
				scanIndex,
				sizeChunks,
				xData_w.data(), yData_w.data(), zData_w.data(),
				_isInvalidData_w.data(),
				intData_w.data(),
				NULL,
				redData_w.data(), greenData_w.data(), blueData_w.data()
			);

			int64_t global_count = 0;
			while (size = dataReader.read()) {
				count = size;
				std::copy(xData.begin(), xData.end(), xData_w.begin());
				std::copy(yData.begin(), yData.end(), yData_w.begin());
				std::copy(zData.begin(), zData.end(), zData_w.begin());
				std::copy(redData.begin(), redData.end(), redData_w.begin());
				std::copy(greenData.begin(), greenData.end(), greenData_w.begin());
				std::copy(blueData.begin(), blueData.end(), blueData_w.begin());
				std::copy(intData.begin(), intData.end(), intData_w.begin());
				std::copy(isInvalidData.begin(), isInvalidData.end(), _isInvalidData_w.begin());

				dataWriter.write(count);
				global_count++;
			}

			
			
			std::vector<int64_t> idElementValue_w;
			std::vector<int64_t> startPointIndex_w;
			vector<int64_t> pointCount_w;
			int group = 0;
			dataWriter.close();
			pointCount_w.push_back(global_count);
			idElementValue_w.push_back(0);
			startPointIndex_w.push_back(0);
			

			eWriter.WriteData3DGroupsData(scanIndex, 1,
				&idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);
		}
		eWriter.Close();
		eReader.Close();

		return 0;
	}

	int Unique(std::string nameIn, std::string nameOut) {

		//reader
		e57::Reader eReader(nameIn);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		std::vector<MyPoint> pts;
		e57::Data3D scanHeader_r;
		eReader.ReadData3D(0, scanHeader_r);

		int64_t sizeChunks = 1024 * 1024 * 10;
		int64_t nColumn = 0;
		int64_t nRow = 0;

		int64_t nPoints = 0;        //Number of points

		int64_t nGroupsSize = 0;        //Number of groups
		int64_t nCountSize = 0;         //Number of points per group
		bool    bColumnIndex = false; //indicates that idElementName is "columnIndex"

		eReader.GetData3DSizes(0, nRow, nColumn, nPoints, nGroupsSize, nCountSize, bColumnIndex);
		if (nRow == 0 && nColumn == 0) {
			nRow = nPoints;
			nColumn = 1;

			nGroupsSize = 1;
			nGroupsSize = 1;
			nCountSize = nPoints;
			bColumnIndex = true;
		}
		int64_t nSize = nRow;

		//writer
		e57::Writer eWriter(nameOut, "");
		e57::Data3D scanHeader_w;
		scanHeader_w.description = "";
		scanHeader_w.acquisitionStart.isAtomicClockReferenced = 0;
		scanHeader_w.name = scanHeader_r.name;

		scanHeader_w.guid = scanHeader_r.guid;
		scanHeader_w.indexBounds.rowMaximum = scanHeader_r.indexBounds.rowMaximum;
		scanHeader_w.indexBounds.rowMinimum = scanHeader_r.indexBounds.rowMinimum;
		scanHeader_w.indexBounds.columnMaximum = scanHeader_r.indexBounds.columnMaximum;
		scanHeader_w.indexBounds.columnMinimum = scanHeader_r.indexBounds.columnMinimum;
		scanHeader_w.indexBounds.returnMaximum = scanHeader_r.indexBounds.returnMaximum;
		scanHeader_w.indexBounds.returnMinimum = scanHeader_r.indexBounds.returnMinimum;

		scanHeader_w.pointGroupingSchemes.groupingByLine.groupsSize = scanHeader_r.pointGroupingSchemes.groupingByLine.groupsSize;
		scanHeader_w.pointGroupingSchemes.groupingByLine.pointCountSize = scanHeader_r.pointGroupingSchemes.groupingByLine.pointCountSize;
		scanHeader_w.pointGroupingSchemes.groupingByLine.idElementName = scanHeader_r.pointGroupingSchemes.groupingByLine.idElementName;

		scanHeader_w.pointsSize = scanHeader_r.pointsSize;
		
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

		scanHeader_w.pointFields.cartesianInvalidStateField = true;


		scanHeader_w.pointFields.colorRedField = true;
		scanHeader_w.pointFields.colorGreenField = true;
		scanHeader_w.pointFields.colorBlueField = true;


		scanHeader_w.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
		scanHeader_w.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
		scanHeader_w.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
		scanHeader_w.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
		scanHeader_w.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
		scanHeader_w.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;

		scanHeader_w.pointFields.intensityField = scanHeader_r.pointFields.intensityField;

		scanHeader_w.intensityLimits.intensityMinimum = 0.;
		scanHeader_w.intensityLimits.intensityMaximum = 1.;
		scanHeader_w.pointFields.intensityScaledInteger = 0.;

		
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

			if (!eReader.ReadData3DGroupsData(0, nGroupsSize, idElementValue.data(),
				startPointIndex.data(), pointCount.data()))
				nGroupsSize = 0;
		}

		int scanIndex = eWriter.NewData3D(scanHeader_w);

		std::vector<int32_t> rowIndex;
		std::vector<int32_t> columnIndex;
		if (scanHeader_r.pointFields.rowIndexField)
			rowIndex.resize(sizeChunks);
		if (scanHeader_r.pointFields.columnIndexField)
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


		std::vector<double> xData_w, yData_w, zData_w;
		std::vector<uint16_t> redData_w, greenData_w, blueData_w;
		std::vector<double>intData_w;
		std::vector<int8_t> isInvalidData_w;
		

		xData_w.resize(sizeChunks);
		yData_w.resize(sizeChunks);
		zData_w.resize(sizeChunks);

		redData_w.resize(sizeChunks);
		greenData_w.resize(sizeChunks);
		blueData_w.resize(sizeChunks);

		intData_w.resize(sizeChunks);
		isInvalidData_w.resize(sizeChunks);
		e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
			0,
			sizeChunks,
			xData_w.data(), 
			yData_w.data(), 
			zData_w.data(),
			isInvalidData_w.data(),
			intData_w.data(),
			NULL,
			redData_w.data(), 
			greenData_w.data(), 
			blueData_w.data()
		);
		unsigned size = 0;
		size_t col = 0;
		size_t  row = 0;
		int64_t count = 0;

		int64_t global_count = 0;
		while (size = dataReader.read()) {
			count = size;
			std::copy(xData.begin(), xData.end(), xData_w.begin());
			std::copy(yData.begin(), yData.end(), yData_w.begin());
			std::copy(zData.begin(), zData.end(), zData_w.begin());
			std::copy(redData.begin(),redData.end(), redData_w.begin());
			std::copy(greenData.begin(), greenData.end(), greenData_w.begin());
			std::copy(blueData.begin(), blueData.end(), blueData_w.begin());
			std::copy(intData.begin(), intData.end(), intData_w.begin());
			std::copy(isInvalidData.begin(), isInvalidData.end(), isInvalidData_w.begin());

			dataWriter.write(count);
			global_count++;
		}
		std::vector<int64_t> idElementValue_w;
		std::vector<int64_t> startPointIndex_w;
		vector<int64_t> pointCount_w;
		int group = 0;
		dataWriter.close();
		pointCount_w.push_back(global_count);
		idElementValue_w.push_back(0);
		startPointIndex_w.push_back(0);
		eWriter.WriteData3DGroupsData(0, 1,
			&idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);



		eReader.Close();
		eWriter.Close();
		return 0;
	}
}


