#include "SaveFactor.hpp"
#include <random>






std::string createGUID(){
	GUID		guid;
	CoCreateGuid((GUID*)&guid);
	OLECHAR wbuffer[64];
	StringFromGUID2(guid, &wbuffer[0], 64);
	size_t	converted = 0;
	char	strGuid[64];
	wcstombs_s(&converted, strGuid, wbuffer, 64);
	return strGuid;
}

using namespace e57;
using namespace std;
void printSomePoints(ImageFile& imf, CompressedVectorNode& points);
int processE57(std::string _nameIn) {
		e57::Reader eReader(_nameIn);

		e57::E57Root rootHeader;
		eReader.GetE57Root(rootHeader);
		std::string guid = rootHeader.guid;
		std::string coordinateMetaData = rootHeader.coordinateMetadata;
		int numberOfScan = eReader.GetData3DCount();
		int64_t numberOfPoints = 0;
		for (int i = 0; i < numberOfScan; ++i) {


			e57::Data3D scanHeader_r;
			int64_t sizeChunks = 1024 * 8;
			eReader.ReadData3D(0, scanHeader_r);

			e57::Data3D& header = scanHeader_r;
			std::string name = header.name;
			std::string guid = header.guid;
			std::string desc = header.description;

			int64_t nRow, nColumn, nPoints, nGroupsSize, nCountsSize;
			bool bColumnIndex;
			eReader.GetData3DSizes(0, nRow, nColumn, nPoints, nGroupsSize, nCountsSize, bColumnIndex);
			if (nRow == 0 && nColumn == 0) {
				nRow = nPoints;
				nColumn = 1;

				nGroupsSize = 1;
				nGroupsSize = 1;
				nCountsSize = nPoints;
				bColumnIndex = true;
			}
			double xTrans = header.pose.translation.x;
			double yTrans = header.pose.translation.y;
			double zTrans = header.pose.translation.z;

			double wRot = header.pose.rotation.w;
			double xRot = header.pose.rotation.x;
			double yRot = header.pose.rotation.y;
			double zRot = header.pose.rotation.z;


			bool bInvalidState = false;
			std::vector<int8_t> isInvalidData_r;
			if (header.pointFields.cartesianInvalidStateField) {
				isInvalidData_r.resize(sizeChunks);
				bInvalidState = true;
			}

			std::vector<double> xData_r, yData_r, zData_r;
			if (header.pointFields.cartesianXField)
				xData_r.resize(sizeChunks);

			if (header.pointFields.cartesianYField)
				yData_r.resize(sizeChunks);

			if (header.pointFields.cartesianZField)
				zData_r.resize(sizeChunks);

			bool bIntens;
			std::vector<double> intData_r;
			int64_t intRange, intOffset;
			if (header.pointFields.intensityField) {
				bIntens = true;
				intData_r.resize(sizeChunks);
				intRange = header.intensityLimits.intensityMaximum - header.intensityLimits.intensityMinimum;
				intOffset = header.intensityLimits.intensityMinimum;
			}

			bool bColor;
			std::vector<uint16_t> redData_r;
			std::vector<uint16_t> greenData_r;
			std::vector<uint16_t> blueData_r;

			int64_t colorRedRange, colorRedOffset, colorGreenRange, colorGreenOffset, colorBlueRange, colorBlueOffset;
			if (header.pointFields.colorRedField)
			{
				bColor = true;
				redData_r.resize(sizeChunks);
				greenData_r.resize(sizeChunks);
				blueData_r.resize(sizeChunks);
				colorRedRange = header.colorLimits.colorRedMaximum - header.colorLimits.colorRedMinimum;
				colorRedOffset = header.colorLimits.colorRedMinimum;
				colorGreenRange = header.colorLimits.colorGreenMaximum - header.colorLimits.colorGreenMinimum;
				colorGreenOffset = header.colorLimits.colorGreenMinimum;
				colorBlueRange = header.colorLimits.colorBlueMaximum - header.colorLimits.colorBlueMinimum;
				colorBlueOffset = header.colorLimits.colorBlueMinimum;
			}

			std::vector<int64_t> idElementValue_r, startPointIndex_r, pointCount_r;
			if (nGroupsSize > 0)
			{
				idElementValue_r.resize(nGroupsSize);
				startPointIndex_r.resize(nGroupsSize);
				pointCount_r.resize(nGroupsSize);

				if (!eReader.ReadData3DGroupsData(0, nGroupsSize, idElementValue_r.data(),
					startPointIndex_r.data(), pointCount_r.data()))
					nGroupsSize = 0;
			}


			std::vector<int32_t> rowIndex_r, columnIndex_r;

			if (header.pointFields.rowIndexField)
				rowIndex_r.resize(sizeChunks);
			if (header.pointFields.columnIndexField)
				columnIndex_r.resize(sizeChunks);


			e57::CompressedVectorReader dataReader = eReader.SetUpData3DPointsData(
				0,
				sizeChunks,
				xData_r.data(),
				yData_r.data(),
				zData_r.data(),
				isInvalidData_r.data(),
				intData_r.data(),
				NULL,
				redData_r.data(),
				greenData_r.data(),
				blueData_r.data(),
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				rowIndex_r.data(),
				columnIndex_r.data()
			);




			//e57::CompressedVectorReader dataReader = initReader(eReader, reader, sizeChunks, i);
			pcl::PointCloud<mypt3d>::Ptr pts(new pcl::PointCloud<mypt3d>);
			pts->points.resize(nRow * nColumn);


			double r00, r10, r20;
			double r01, r11, r21;
			double r02, r12, r22;
			if (true) {
				r00 = 1. - 2. * yRot * yRot - 2. * zRot * zRot;
				r10 = 2. * xRot * yRot - 2. * zRot * wRot;
				r20 = 2. * xRot * zRot + 2. * yRot * wRot;

				r01 = 2. * xRot *yRot + 2. * zRot * wRot;
				r11 = 1. - 2. * xRot * xRot - 2. * zRot * zRot;
				r21 = 2. * yRot * zRot - 2. * xRot * wRot;

				r02 = 2. * xRot * zRot - 2. * yRot * wRot;
				r12 = 2. * yRot * zRot + 2. * xRot * wRot;
				r22 = 1. - 2. * xRot * xRot - 2. * yRot * yRot;

			}

			unsigned size = 0;
			size_t col = 0;
			size_t  row = 0;
			int64_t count = 0;

			int64_t exSize_PTS = 0;
			while (size = dataReader.read()) {
				for (int64_t i = 0; i < size; ++i) {
					if (columnIndex_r.data())
						col = columnIndex_r[i];
					else
						col = 0;        //point cloud case

					if (rowIndex_r.data())
						row = rowIndex_r[i];
					else
						row = count;    //point cloud case

					size_t index = row * nColumn + col;

					//std::cout << index << std::endl;
					mypt3d& pt = pts->points[i];
					if (true) { // les points doivent être modifiés car on va les assembler, on va donc perdre l'information du centre du scan.
						if (bInvalidState) {
							if (isInvalidData_r[i] == 0) {
								pt.x = xData_r[i] * r00 + yData_r[i] * r10 + zData_r[i] * r20 + xTrans;
								pt.y = xData_r[i] * r01 + yData_r[i] * r11 + zData_r[i] * r21 + yTrans;
								pt.z = xData_r[i] * r02 + yData_r[i] * r12 + zData_r[i] * r22 + zTrans;
							}
						}
						else
						{
							pt.x = xData_r[i] * r00 + yData_r[i] * r10 + zData_r[i] * r20 + xTrans;
							pt.y = xData_r[i] * r01 + yData_r[i] * r11 + zData_r[i] * r21 + yTrans;
							pt.z = xData_r[i] * r02 + yData_r[i] * r12 + zData_r[i] * r22 + zTrans;
						}
					}
					else { //On va pas changer 
						if (bInvalidState) {
							if (isInvalidData_r[i] == 0) {
								pt.x = xData_r[i];
								pt.y = yData_r[i];
								pt.z = zData_r[i];
							}
						}
						else
						{
							pt.x = xData_r[i];
							pt.y = yData_r[i];
							pt.z = zData_r[i];
						}
					}
					if (bIntens) {         //Normalize intensity to 0 - 1.
						double intensity = (intData_r[i] - intOffset) / intRange;
						pt.intensity = intensity;
					}

					if (bColor) {                     //Normalize color to 0 - 255
						int red = ((redData_r[i] - colorRedOffset) * 255) / colorRedRange;
						int green = ((greenData_r[i] - colorGreenOffset) * 255) / colorBlueRange;
						int blue = ((blueData_r[i] - colorBlueOffset) * 255) / colorBlueRange;
						pt.r = red;
						pt.g = green;
						pt.b = blue;
					}
					count++;
				}
			}
			return count;
		}
	
}
//
//int main(int argc, char* argv[]) {
//	std::random_device rd;
//	std::mt19937 mt(rd());
//	std::uniform_real_distribution<double> dist(0.0, 1000.0);
//
//
//	//SavableFile *file = SaveFactor::get("test.a");
//	//SavableFile *file = new SaveE57("test.e57");
//	//file->writeHeader();
//	
//	long int size = 1024 * 1024 * 8;
//	std::vector<mypt3d> pts;
//
//	for (int i = 0; i < size; ++i) {
//		float i_size = i / size;
//		float imodsize = i % size;
//		pts.push_back(mypt3d(/*x*/dist(mt) /*random 0 -> 1000*/, /*y*/dist(mt)/*random 0 -> 1000*/, /*z*/0, /*intensity*/i_size, /*r*/imodsize, /*g*/imodsize, /*b*/imodsize));
//	}
//
//	/******************/
//
//	std::string p_name = "test.e57";
//	e57::Writer eWriter(p_name, "");
//	e57::Data3D scanHeader;
//	e57WriteData datas;
//
//	long int pointCount = 1024 * 1024 * 8;
//
//
//	scanHeader.name = p_name;
//	scanHeader.description = "e57 file";
//
//	/*create GUID*/
//	GUID guid;
//	CoCreateGuid((GUID*)&guid);
//	OLECHAR wbuffer[64];
//	StringFromGUID2(guid, &wbuffer[0], 64);
//	size_t converted = 0;
//	char strGuid[64];
//	wcstombs_s(&converted, strGuid, wbuffer, 64);
//	scanHeader.guid = (char*)strGuid;
//	/*Fin*/
//	scanHeader.indexBounds.rowMaximum = pointCount;
//	scanHeader.indexBounds.rowMinimum = 0;
//	scanHeader.indexBounds.columnMaximum = 0;
//	scanHeader.indexBounds.columnMinimum = 0;
//	scanHeader.indexBounds.returnMaximum = 0;
//	scanHeader.indexBounds.returnMinimum = 0;
//
//	scanHeader.pointGroupingSchemes.groupingByLine.groupsSize = 1;
//	scanHeader.pointGroupingSchemes.groupingByLine.pointCountSize = pointCount;
//	scanHeader.pointGroupingSchemes.groupingByLine.idElementName = "columnIndex";
//
//	scanHeader.pointsSize = pointCount;
//
//	scanHeader.pose.rotation.w = 1;
//	scanHeader.pose.rotation.x = 0;
//	scanHeader.pose.rotation.y = 0;
//	scanHeader.pose.rotation.z = 0;
//	scanHeader.pose.translation.x = 0;
//	scanHeader.pose.translation.y = 0;
//	scanHeader.pose.translation.z = 0;
//
//	scanHeader.pointFields.cartesianXField = true;
//	scanHeader.pointFields.cartesianYField = true;
//	scanHeader.pointFields.cartesianZField = true;
//	scanHeader.pointFields.cartesianInvalidStateField = true;
//
//	scanHeader.pointFields.colorRedField = true;
//	scanHeader.pointFields.colorGreenField = true;
//	scanHeader.pointFields.colorBlueField = true;
//
//	scanHeader.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
//	scanHeader.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
//	scanHeader.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
//	scanHeader.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
//	scanHeader.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
//	scanHeader.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;
//
//	scanHeader.pointFields.intensityField = true;
//
//	scanHeader.intensityLimits.intensityMinimum = 0.;
//	scanHeader.intensityLimits.intensityMaximum = 1.;
//	scanHeader.pointFields.intensityScaledInteger = 0.;
//
//	scanHeader.pointsSize = pointCount;
//	int scanIndex =  eWriter.NewData3D(scanHeader);
//
//	datas.xData.resize(pointCount);
//	datas.yData.resize(pointCount);
//	datas.zData.resize(pointCount);
//	datas.isInvalidData.resize(pointCount);
//	datas.intData.resize(pointCount);
//	datas.redData.resize(pointCount);
//	datas.greenData.resize(pointCount);
//	datas.blueData.resize(pointCount);
//	int i = 0;
//
//	e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
//		scanIndex,
//		pointCount,
//		datas.xData.data(),
//		datas.yData.data(),
//		datas.zData.data(),
//		datas.isInvalidData.data(),
//		datas.intData.data(),
//		NULL,
//		datas.redData.data(),
//		datas.greenData.data(),
//		datas.blueData.data()
//	);
//
//
//
//	for (; i < size; ++i) {
//		mypt3d& pt = pts[i];
//		datas.xData[i] = pt.x;
//		datas.yData[i] = pt.y;
//		datas.zData[i] = pt.z;
//
//
//		if (pt.intensity <= 0)
//			datas.intData[i] = 0.1;
//		else if (pt.intensity > 1)
//			datas.intData[i] = pt.intensity / 255.f;
//
//		datas.redData[i] = pt.r;
//		datas.greenData[i] = pt.g;
//		datas.blueData[i] = pt.b;
//
//		if ((pt.r <= 0 && pt.g <= 0 && pt.b <= 0) || (pt.r >= 255 && pt.g >= 255 && pt.b >= 255)) {
//			datas.redData[i] = pt.intensity * 255;
//			datas.greenData[i] = pt.intensity * 255;
//			datas.blueData[i] = pt.intensity * 255;
//		}
//
//
//		/*************/
//		datas.redData[i] = 127;
//		datas.greenData[i] = 127;
//		datas.blueData[i] = 127;
//		datas.intData[i] = 0.5;
//
//		datas.isInvalidData[i] = 0;
//	}
//	dataWriter.write(i);
//	dataWriter.close();
//
//	std::vector<int64_t> idElementValue_w;
//	std::vector<int64_t> startPointIndex_w;
//	std::vector<int64_t> pointCount_w;
//
//	idElementValue_w.push_back(0);
//	startPointIndex_w.push_back(0);
//	pointCount_w.push_back(pointCount);
//	eWriter.WriteData3DGroupsData(scanIndex, 1, &idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);
//
//
//
//
//
//
//
//
//
//	///*****************************/
//	//file->write(pts1);
//	///*file->write(pts2);
//	//file->write(pts3);*/
//	//file->writeFooter();
//
//
//	processE57("test.e57");
//}

//using namespace e57;
//using namespace std;
//
//int main(int /*argc*/, char** /*argv*/) {
//	try {
//		ImageFile imf("temp._e57", "w");
//		StructureNode root = imf.root();
//
//		root.set("greeting", StringNode(imf, "Hello world."));
//
//		imf.close(); // don't forget to explicitly close the ImageFile
//	}
//	catch (E57Exception& ex) {
//		ex.report(__FILE__, __LINE__, __FUNCTION__);
//		return(-1);
//	}
//
//	try {
//		ImageFile imf("temp._e57", "r");
//		StructureNode root = imf.root();
//
//		StringNode greeting(root.get("greeting"));
//		cout << "Value of /greeting = " << greeting.value() << endl;
//
//		imf.close(); // don't forget to explicitly close the ImageFile
//	}
//	catch (E57Exception& ex) {
//		ex.report(__FILE__, __LINE__, __FUNCTION__);
//		return(-1);
//	}
//	return(0);
//}


int main(int /*argc*/, char** /*argv*/)
{

	//processE57("context_filtered.e57");
	//processE57("foo.e57");


	std::vector<mypt3d> pts1, pts2;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1000.0);
	
	
	//SavableFile *file = SaveFactor::get("test.a");
	//SavableFile *file = new SaveE57("test.e57");
	//file->writeHeader();
		
	long int size = 1024 * 512;
	
	for (int i = 0; i < size; ++i) {
		float i_size = i / size;
		int imodsize = i % 255;
		pts1.push_back(mypt3d(/*x*/dist(mt) /*random 0 -> 1000*/, /*y*/dist(mt)/*random 0 -> 1000*/, /*z*/0, /*intensity*/0.7, /*r*/0, /*g*/imodsize, /*b*/imodsize));
		pts2.push_back(mypt3d(/*x*/dist(mt) /*random 0 -> 1000*/, /*y*/dist(mt)/*random 0 -> 1000*/, /*z*/100, /*intensity*/0.2, /*r*/imodsize, /*g*/imodsize, /*b*/imodsize));
	}

	try {
		/// Open new file for writing, get the initialized root node (a Structure).
		/// Path name: "/"
		ImageFile imf("foo.e57", "w");
		StructureNode root = imf.root();
		imf.extensionsAdd("demo", "http://www.example.com/DemoExtension");
		/// Set per-file properties.
		/// Path names: "/formatName", "/majorVersion", "/minorVersion", "/coordinateMetadata"
		root.set("formatName", StringNode(imf, "ASTM E57 3D Imaging Data File"));
		root.set("guid", StringNode(imf, "3F2504E0-4F89-11D3-9A0C-0305E82C3300"));

		/// Get ASTM version number supported by library, so can write it into file
		int astmMajor;
		int astmMinor;
		ustring libraryId;
		E57Utilities().getVersions(astmMajor, astmMinor, libraryId);
		root.set("versionMajor", IntegerNode(imf, astmMajor));
		root.set("versionMinor", IntegerNode(imf, astmMinor));
		root.set("e57LibraryVersion", StringNode(imf, "Octhread"));
		/// Save a dummy string for coordinate system.
		/// Really should be a valid WKT string identifying the coordinate reference system (CRS).
		root.set("coordinateMetadata", StringNode(imf, ""));


		/// Create 3D data area.
		/// Path name: "/data3D"
		VectorNode data3D = VectorNode(imf, true);
		root.set("data3D", data3D);

		/// Add first scan
		/// Path name: "/data3D/0"
		StructureNode scan0 = StructureNode(imf);
		data3D.append(scan0);

		/// Add guid to scan0.
		/// Path name: "/data3D/0/guid".
		std::string test = createGUID();
		const char* scanGuid0 = "3F2504E0-4F89-11D3-9A0C-0305E82C3301";
		scan0.set("guid", StringNode(imf, scanGuid0));



		///*Index bound*/
		StructureNode indexBounds = StructureNode(imf);

		indexBounds.set("columnMaximum", IntegerNode(imf, 1, 0, size * 4));
		indexBounds.set("columnMinimum", IntegerNode(imf, 0, 0, 0));
		indexBounds.set("rowMaximum", IntegerNode(imf, size * 2, 0, size * 4));
		indexBounds.set("rowMinimum", IntegerNode(imf, 0, 0, 0));
		scan0.set("indexBounds", indexBounds);




		/// Make a prototype of datatypes that will be stored in points record.
		/// This prototype will be used in creating the points CompressedVector.
		/// Using this proto in a CompressedVector will define path names like:
		///      "/data3D/0/points/0/cartesianX"
		StructureNode proto = StructureNode(imf);
		proto.set("cartesianX", ScaledIntegerNode(imf, 0, 0, 32767, 1, 0));
		proto.set("cartesianY", ScaledIntegerNode(imf, 0, 0, 32767, 1, 0));
		proto.set("cartesianZ", ScaledIntegerNode(imf, 0, 0, 32767, 1, 0));
		proto.set("cartesianInvalidState", IntegerNode(imf, 0, 0, 2));
		proto.set("intensity", FloatNode(imf, 0.0, E57_SINGLE, 0.0, 1.0));
		proto.set("colorRed", IntegerNode(imf, 0, 0, 255));
		proto.set("colorGreen", IntegerNode(imf, 0, 0, 255));
		proto.set("colorBlue", IntegerNode(imf, 0, 0, 255));

		/// Make empty codecs vector for use in creating points CompressedVector.
		/// If this vector is empty, it is assumed that all fields will use the BitPack codec.
		VectorNode codecs = VectorNode(imf, true);

		/// Create CompressedVector for storing points.  Path Name: "/data3D/0/points".
		/// We use the prototype and empty codecs tree from above.
		/// The CompressedVector will be filled by code below.
		CompressedVectorNode points = CompressedVectorNode(imf, proto, codecs);
		scan0.set("points", points);

		/// Create pose structure for scan.
		/// Path names: "/data3D/0/pose/rotation/w", etc...
		///             "/data3D/0/pose/translation/x", etc...
		StructureNode pose = StructureNode(imf);
		scan0.set("pose", pose);
		StructureNode rotation = StructureNode(imf);
		pose.set("rotation", rotation);
		rotation.set("w", FloatNode(imf, 1.0));
		rotation.set("x", FloatNode(imf, 0.0));
		rotation.set("y", FloatNode(imf, 0.0));
		rotation.set("z", FloatNode(imf, 0.0));
		StructureNode translation = StructureNode(imf);
		pose.set("translation", translation);
		translation.set("x", FloatNode(imf, 0.0));
		translation.set("y", FloatNode(imf, 0.0));
		translation.set("z", FloatNode(imf, 0.0));

		///================
		/// Add grouping scheme area
		/// Path name: "/data3D/0/pointGroupingSchemes"
		StructureNode pointGroupingSchemes = StructureNode(imf);
		scan0.set("pointGroupingSchemes", pointGroupingSchemes);

		/// Add a line grouping scheme
		/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine"
		StructureNode groupingByLine = StructureNode(imf);
		pointGroupingSchemes.set("groupingByLine", groupingByLine);

		/// Add idElementName to groupingByLine, specify a line is column oriented
		/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine/idElementName"
		groupingByLine.set("idElementName", StringNode(imf, "columnIndex"));

		/// Make a prototype of datatypes that will be stored in LineGroupRecord.
		/// This prototype will be used in creating the groups CompressedVector.
		/// Will define path names like:
		///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/idElementValue"
		StructureNode lineGroupProto = StructureNode(imf);
		lineGroupProto.set("idElementValue", IntegerNode(imf, 0, 0));
		lineGroupProto.set("startPointIndex", IntegerNode(imf, 0, 0 ));
		lineGroupProto.set("pointCount", IntegerNode(imf, 1, 0, size * 10));

		/// Add cartesian bounds to line group prototype
		/// Will define path names like:
		///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/cartesianBounds/xMinimum"
		StructureNode lineGroupBbox = StructureNode(imf);
		lineGroupProto.set("cartesianBounds", lineGroupBbox);
		lineGroupBbox.set("xMinimum", FloatNode(imf, 0.0));
		lineGroupBbox.set("xMaximum", FloatNode(imf, 0.0));
		lineGroupBbox.set("yMinimum", FloatNode(imf, 0.0));
		lineGroupBbox.set("yMaximum", FloatNode(imf, 0.0));
		lineGroupBbox.set("zMinimum", FloatNode(imf, 0.0));
		lineGroupBbox.set("zMaximum", FloatNode(imf, 0.0));

		/// Make empty codecs vector for use in creating groups CompressedVector.
		/// If this vector is empty, it is assumed that all fields will use the BitPack codec.
		VectorNode lineGroupCodecs = VectorNode(imf, true);

		/// Create CompressedVector for storing groups.  
		/// Path Name: "/data3D/0/pointGroupingSchemes/groupingByLine/groups".
		/// We use the prototype and empty codecs tree from above.
		/// The CompressedVector will be filled by code below.
		CompressedVectorNode groups = CompressedVectorNode(imf, lineGroupProto, lineGroupCodecs);
		groupingByLine.set("groups", groups);


		/// Add Cartesian bounding box to scan.
/// Path names: "/data3D/0/cartesianBounds/xMinimum", etc...
		StructureNode bbox = StructureNode(imf);
		bbox.set("xMinimum", FloatNode(imf, 0.0));
		bbox.set("xMaximum", FloatNode(imf, 1000.0));
		bbox.set("yMinimum", FloatNode(imf, 0.0));
		bbox.set("yMaximum", FloatNode(imf, 1000.0));
		bbox.set("zMinimum", FloatNode(imf, 0.0));
		bbox.set("zMaximum", FloatNode(imf, 1000.0));
		scan0.set("cartesianBounds", bbox);
		///================
		/// Add name and description to scan
		/// Path names: "/data3D/0/name", "/data3D/0/description".
		scan0.set("name", StringNode(imf, "MyTest"));
		scan0.set("description", StringNode(imf, ""));



		///================VectorNode images2D = VectorNode(imf, true);

		VectorNode images2D = VectorNode(imf, true);
		root.set("images2D", images2D);

		long int size_2 = size / 3;
		std::vector<double> cartesianX; cartesianX.resize(size_2);
		std::vector<double> cartesianY; cartesianY.resize(size_2);
		std::vector<double> cartesianZ; cartesianZ.resize(size_2);
		std::vector<double> cartesianInvalidState; cartesianInvalidState.resize(size_2);
		std::vector<double> intensity; intensity.resize(size_2);
		std::vector<int> red; red.resize(size_2);
		std::vector<int> green; green.resize(size_2);
		std::vector<int> blue; blue.resize(size_2);

		vector<SourceDestBuffer> sourceBuffers;
		sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianX", cartesianX.data(), size_2, true, true));
		sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianY", cartesianY.data(), size_2, true, true));
		sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianZ", cartesianZ.data(), size_2, true, true));
		sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianInvalidState", cartesianInvalidState.data(), size_2, true));
		sourceBuffers.push_back(SourceDestBuffer(imf, "intensity", intensity.data(), size_2, true));
		sourceBuffers.push_back(SourceDestBuffer(imf, "colorRed", red.data(), size_2, true));
		sourceBuffers.push_back(SourceDestBuffer(imf, "colorGreen", green.data(), size_2, true));
		sourceBuffers.push_back(SourceDestBuffer(imf, "colorBlue", blue.data(), size_2, true));
		long int count = 0;

		CompressedVectorWriter writer = points.writer(sourceBuffers);


		//Pts => size = size
		//tant que le compte totale qu'on a ajouter n'es pas égale a la taille totale du tableau
		while (count < pts1.size()) {
			int i = 0;
			while (i < size_2 && count < pts1.size()) {
				cartesianX[i] = pts1[count].x;
				cartesianY[i] = pts1[count].y;
				cartesianZ[i] = pts1[count].z;
				red[i] = pts1[count].r;
				green[i] = pts1[count].g;
				blue[i] = pts1[count].b;
				intensity[i] = pts1[count].intensity;

				cartesianInvalidState[i] = 0;

				count++;
				++i;
			}
			if (i!=0)
				writer.write(i);
		}

		long int count2 = 0;
		while (count2 < pts2.size()) {
			int i = 0;
			while (i < size_2 && count2 < pts2.size()) {
				cartesianX[i] = pts2[count2].x;
				cartesianZ[i] = pts2[count2].z;
				cartesianY[i] = pts2[count2].y;
				red[i] = pts2[count2].r;
				green[i] = pts2[count2].g;
				blue[i] = pts2[count2].b;
				intensity[i] = pts2[count2].intensity;

				cartesianInvalidState[i] = 0;

				count2++;
				count++;
				++i;
			}
			if (i != 0)
				writer.write(i);
		}


		writer.close();

		/// Write source buffers into CompressedVector
	

		//ça ça marche bien! 
		//Il suffit de faire des buffers de toujours la même taille et on est bon 




		///================
		/// Prepare vector of source buffers for writing in the CompressedVector of groups
		const int NG = 1;
		int32_t idElementValue[NG] = { 0 };		
		int32_t startPointIndex[NG] = { 0 };
		int32_t pointCount[NG] = { count };
		double  xMinimum[NG] = { 0.0 };
		double  xMaximum[NG] = { 20 };
		double  yMinimum[NG] = { 0 };
		double  yMaximum[NG] = { 20 };
		double  zMinimum[NG] = { 0 };
		double  zMaximum[NG] = { 20 };

		vector<SourceDestBuffer> groupSDBuffers;
		groupSDBuffers.push_back(SourceDestBuffer(imf, "idElementValue", idElementValue, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "startPointIndex", startPointIndex, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "pointCount", pointCount, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "cartesianBounds/xMinimum", xMinimum, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "cartesianBounds/xMaximum", xMaximum, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "cartesianBounds/yMinimum", yMinimum, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "cartesianBounds/yMaximum", yMaximum, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "cartesianBounds/zMinimum", zMinimum, NG, true));
		groupSDBuffers.push_back(SourceDestBuffer(imf, "cartesianBounds/zMaximum", zMaximum, NG, true));

		/// Write source buffers into CompressedVector
		{
			CompressedVectorWriter writer = groups.writer(groupSDBuffers);
			writer.write(NG);
			writer.close();
		}

		imf.close();
	}

	catch (E57Exception& ex) {
		ex.report(__FILE__, __LINE__, __FUNCTION__);
	}
	catch (std::exception& ex) {
		cerr << "Got an std::exception, what=" << ex.what() << endl;
	}
	catch (...) {
		cerr << "Got an unknown exception" << endl;
	}

	processE57("foo.e57");


	//processE57("foo.e57");
	try {





		/// Read file from disk
		ImageFile imf("foo.e57", "r");
		StructureNode root = imf.root();

		/// Make sure vector of scans is defined and of expected type.
		/// If "/data3D" wasn't defined, the call to root.get below would raise an exception.
		if (!root.isDefined("/data3D")) {
			cout << "File doesn't contain 3D images" << endl;
			return 0;
		}
		Node n = root.get("/data3D");
		if (n.type() != E57_VECTOR) {
			cout << "bad file" << endl;
			return 0;
		}

		/// The node is a vector so we can safely get a VectorNode handle to it.
		/// If n was not a VectorNode, this would raise an exception.
		VectorNode data3D(n);

		/// Print number of children of data3D.  This is the number of scans in file.
		int64_t scanCount = data3D.childCount();
		cout << "Number of scans in file:" << scanCount << endl;

		/// For each scan, print out first 4 points in either Cartesian or Spherical coordinates.
		for (int scanIndex = 0; scanIndex < scanCount; scanIndex++) {
			/// Get scan from "/data3D", assume its a Structure (else get exception)
			StructureNode scan(data3D.get(scanIndex));
			cout << "got:" << scan.pathName() << endl;

			/// Get "points" field in scan.  Should be a CompressedVectorNode.
			CompressedVectorNode points(scan.get("points"));
			cout << "got:" << points.pathName() << endl;

			/// Call subroutine in this file to print the points
			printSomePoints(imf, points);
		}

		imf.close();
	}
	catch (E57Exception& ex) {
		ex.report(__FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	catch (std::exception& ex) {
		cerr << "Got an std::exception, what=" << ex.what() << endl;
		return -1;
	}
	catch (...) {
		cerr << "Got an unknown exception" << endl;
		return -1;
	}

	//processE57("foo.e57");
	return(0);
}




void printSomePoints(ImageFile& imf, CompressedVectorNode& points)
{
	/// Need to figure out if has Cartesian or spherical coordinate system.
	/// Interrogate the CompressedVector's prototype of its records.
	StructureNode proto(points.prototype());

	/// The prototype should have a field named either "cartesianX" or "sphericalRange".
	if (proto.isDefined("cartesianX") && proto.isDefined("cartesianY") && proto.isDefined("cartesianZ")) {
#if 1  //??? pick one?
		/// Make a list of buffers to receive the xyz values.
		const int N = 512;
		vector<SourceDestBuffer> destBuffers;
		double x[N];     destBuffers.push_back(SourceDestBuffer(imf, "cartesianX", x, N, true));
		double y[N];     destBuffers.push_back(SourceDestBuffer(imf, "cartesianY", y, N, true));
		double z[N];     destBuffers.push_back(SourceDestBuffer(imf, "cartesianZ", z, N, true));
		//double r[N];     destBuffers.push_back(SourceDestBuffer(imf, "colorRed", r, N, true));
		//double g[N];     destBuffers.push_back(SourceDestBuffer(imf, "colorGreen", g, N, true));
		//double b[N];     destBuffers.push_back(SourceDestBuffer(imf, "colorBlue",  b, N, true));
		double intens[N];     destBuffers.push_back(SourceDestBuffer(imf, "intensity", intens, N, true));

		/// Create a reader of the points CompressedVector, try to read first block of N points
		/// Each call to reader.read() will fill the xyz buffers until the points are exhausted.
		CompressedVectorReader reader = points.reader(destBuffers);
		//unsigned gotCount = reader.read();
		//cout << "  got first " << gotCount << " points" << endl;

		/// Print the coordinates we got
		int cpt = 0;
		while (int size = reader.read()) {/*
			for (unsigned i = 0; i < size; i++)
				cout << "  " << i << ". x=" << x[i] << " y=" << y[i] << " z=" << z[i] << endl;*/
			cpt += size;
		}
		std::cout << cpt << std::endl;


#else
		/// Make a list of buffers to receive the xyz values.
		
		vector<SourceDestBuffer> destBuffers;
		int64_t columnIndex[10];     destBuffers.push_back(SourceDestBuffer(imf, "columnIndex", columnIndex, 10, true));
		double x[10]; destBuffers.push_back(SourceDestBuffer(imf, "cartesianX", x, 10, true));
		double y[10]; destBuffers.push_back(SourceDestBuffer(imf, "cartesianY", y, 10, true));
		double z[10]; destBuffers.push_back(SourceDestBuffer(imf, "cartesianZ", z, 10, true));

		/// Create a reader of the points CompressedVector, try to read first block of 4 columnIndex
		/// Each call to reader.read() will fill the xyz buffers until the points are exhausted.
		CompressedVectorReader reader = points.reader(destBuffers);
		unsigned gotCount = reader.read();
		cout << "  got first " << gotCount << " points" << endl;
		/// Print the coordinates we got
		for (unsigned i = 0; i < gotCount; i++)
			cout << "  " << i << ". columnIndex=" << columnIndex[i] << endl;
#endif
		reader.close();
	}
	else if (proto.isDefined("sphericalRange")) {
		//??? not implemented yet
	}
	else
		cout << "Error: couldn't find either Cartesian or spherical points in scan" << endl;
}