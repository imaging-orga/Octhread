#pragma once
#include <vector>
#include "../Octhread/base.hpp"
#include "../Octhread/MyFile.h"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"
namespace FILEWITHOUTPOINTS {
	bool RemovePointe57(std::string nameIn, std::string nameOut) {
		//Lire le fichier d'entrée

		e57::Reader eReader(nameIn);
		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		int numberOfScan = eReader.GetData3DCount();

		//writer
		e57::Writer eWriter(nameOut, "");
		e57::Data3D scanHeader;
		scanHeader.name = nameOut;
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
				mypt3d p = mypt3d(0, 0, 0, 0.5, 128, 128, 128);

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

		return true;
	}
}