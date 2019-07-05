#include "SaveE57.h"



//On écrit dans un fichier auxiliaire avant de tout réécrire dans le fichier e57?
SaveE57::SaveE57(std::string _filename) : SavableFile(_filename), eWriter(_filename, ""), sizeChunks{ 1024 * 1024 * 62 }

{
	scanIndex = writeHeader();
	/*sizeChunks = 1024 * 1024 * 100;
	scanIndex = writeHeader();*/
	/*e57::CompressedVectorWriter*//* dataWriter = eWriter.SetUpData3DPointsData(
		scanIndex,
		sizeChunks,
		datas.xData.data(),
		datas.yData.data(),
		datas.zData.data(),
		datas.isInvalidData.data(),
		datas.intData.data(),
		NULL,
		datas.redData.data(),
		datas.greenData.data(),
		datas.blueData.data()
	);*/
}




int SaveE57::writeHeader() {
	e57::Data3D scanHeader;
	scanHeader.name = p_name;
	scanHeader.description = "e57 file";

	/*create GUID*/
	GUID guid;
	CoCreateGuid((GUID*)&guid);
	OLECHAR wbuffer[64];
	StringFromGUID2(guid, &wbuffer[0], 64);
	size_t converted = 0;
	char strGuid[64];
	wcstombs_s(&converted, strGuid, wbuffer, 64);
	scanHeader.guid = (char*)strGuid;
	/*Fin*/
	scanHeader.indexBounds.rowMaximum = pointCount;
	scanHeader.indexBounds.rowMinimum = 0;
	scanHeader.indexBounds.columnMaximum = 0;
	scanHeader.indexBounds.columnMinimum = 0;
	scanHeader.indexBounds.returnMaximum = 0;
	scanHeader.indexBounds.returnMinimum = 0;

	scanHeader.pointGroupingSchemes.groupingByLine.groupsSize = 1;
	scanHeader.pointGroupingSchemes.groupingByLine.pointCountSize = pointCount;
	scanHeader.pointGroupingSchemes.groupingByLine.idElementName = "columnIndex";

	scanHeader.pointsSize = pointCount;

	scanHeader.pose.rotation.w = 1;
	scanHeader.pose.rotation.x = 0;
	scanHeader.pose.rotation.y = 0;
	scanHeader.pose.rotation.z = 0;
	scanHeader.pose.translation.x = 0;
	scanHeader.pose.translation.y = 0;
	scanHeader.pose.translation.z = 0;

	scanHeader.pointFields.cartesianXField = true;
	scanHeader.pointFields.cartesianYField = true;
	scanHeader.pointFields.cartesianZField = true;
	scanHeader.pointFields.cartesianInvalidStateField = true;

	scanHeader.pointFields.colorRedField = true;
	scanHeader.pointFields.colorGreenField = true;
	scanHeader.pointFields.colorBlueField = true;

	scanHeader.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
	scanHeader.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
	scanHeader.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
	scanHeader.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
	scanHeader.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
	scanHeader.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;

	scanHeader.pointFields.intensityField = true;

	scanHeader.intensityLimits.intensityMinimum = 0.;
	scanHeader.intensityLimits.intensityMaximum = 1.;
	scanHeader.pointFields.intensityScaledInteger = 0.;


	datas.xData.resize(sizeChunks);
	datas.yData.resize(sizeChunks);
	datas.zData.resize(sizeChunks);
	datas.intData.resize(sizeChunks);
	datas.isInvalidData.resize(sizeChunks);
	datas.redData.resize(sizeChunks);
	datas.greenData.resize(sizeChunks);
	datas.blueData.resize(sizeChunks);
	scanHeader.pointsSize = 1024 * 1024 * 8 * 4;
	return eWriter.NewData3D(scanHeader);
}

//write va écrire bloc par bloc dans le fichier
int SaveE57::write(std::vector<mypt3d>& pts) 
{
	//Read le fichier binaire

	size_t size = pts.size();
	datas.xData.resize(size);
	datas.yData.resize(size);
	datas.zData.resize(size);
	datas.isInvalidData.resize(size);
	datas.intData.resize(size);
	datas.redData.resize(size);
	datas.greenData.resize(size);
	datas.blueData.resize(size);
	int i = 0;
	e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
		scanIndex,
		size,
		datas.xData.data(),
		datas.yData.data(),
		datas.zData.data(),
		datas.isInvalidData.data(),
		datas.intData.data(),
		NULL,
		datas.redData.data(),
		datas.greenData.data(),
		datas.blueData.data()
	);
	for (; i < size; ++i) {
		mypt3d& pt = pts[i];
		datas.xData[i] = pt.x;
		datas.yData[i] = pt.y;
		datas.zData[i] = pt.z;


		if (pt.intensity <= 0)
			datas.intData[i] = 0.1;
		else if (pt.intensity > 1)
			datas.intData[i] = pt.intensity / 255.f;

			datas.redData[i] = pt.r;
			datas.greenData[i] = pt.g;
			datas.blueData[i] = pt.b;

		if ((pt.r <= 0 && pt.g <= 0 && pt.b <= 0) || (pt.r >= 255 && pt.g >= 255 && pt.b >= 255)) {
			datas.redData[i] = pt.intensity * 255;
			datas.greenData[i] = pt.intensity * 255;
			datas.blueData[i] = pt.intensity * 255;
		}


		/*************/
		datas.redData[i] = 127;
		datas.greenData[i] = 127;
		datas.blueData[i] = 127;
		datas.intData[i] = 0.5;

		datas.isInvalidData[i] = 0;
	}
	dataWriter.write(i);
	dataWriter.close();

	//On resize les data.xData...
	//On écrit les points dans les bon datas
	
	//puis on write
	return 1;
}





int SaveE57::writeFooter()
{
	//dataWriter.close();
	std::vector<int64_t> idElementValue_w;
	std::vector<int64_t> startPointIndex_w;
	std::vector<int64_t> pointCount_w;

	idElementValue_w.push_back(0);
	startPointIndex_w.push_back(0);
	pointCount_w.push_back(pointCount);
	eWriter.WriteData3DGroupsData(scanIndex, 1, &idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);
	return 1;
}

SaveE57::~SaveE57()
{
}
