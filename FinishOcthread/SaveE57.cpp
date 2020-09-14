#include "SaveE57.h"
#define E57_MAX_VERBOSE 1



//On écrit dans un fichier auxiliaire avant de tout réécrire dans le fichier e57?
SaveE57::SaveE57(std::string _filename, long long int _numMax, BoundingBox& _bb) : SavableFile(_filename, _numMax, _bb), eWriter(_filename, "")
{
	//Bien penser changer pointCount dans le loader
	pointCount = _numMax/* = 1024 * 512*/;
	writerChunckSize = std::min((long long)1024*1024*8, _numMax / 3);

	writeHeader();
}

SaveE57::~SaveE57() {
	//imf->close();
	//delete groups;
	//delete imf;
	//delete writer;
}
const e57::ustring createGUID() {
	std::string str = "ABCDEF0123456789";
	std::string ret = "";
	std::random_device rd{};
	std::mt19937 engine{ rd() };

	std::uniform_int_distribution<int>dist{ 0, (int)str.length() - 1 };
	for (int i = 0; i < 36; ++i) {
		if (i == 8 || i == 13 || i == 18 || i == 23) {
			ret += "-";
		}
		else {
			int rnd = dist(engine);
			ret += str[rnd];
		}
	}
	return ret;
}
int SaveE57::writeHeader() {
	eWriter = e57::Writer(p_name, "");
	e57::Data3D scanHeader_w;

	scanHeader_w.name = p_name;
	scanHeader_w.guid = createGUID();

	scanHeader_w.indexBounds.rowMaximum = pointCount;
	scanHeader_w.indexBounds.rowMinimum = 0;
	scanHeader_w.indexBounds.columnMaximum = 1;
	scanHeader_w.indexBounds.columnMinimum = 0;
	scanHeader_w.indexBounds.returnMaximum = 0;
	scanHeader_w.indexBounds.returnMinimum = 0;

	scanHeader_w.pointGroupingSchemes.groupingByLine.groupsSize = 1;
	scanHeader_w.pointGroupingSchemes.groupingByLine.pointCountSize = pointCount;
	scanHeader_w.pointGroupingSchemes.groupingByLine.idElementName = "columnIndex";
	scanHeader_w.pointsSize = pointCount;
	scanHeader_w.pose.rotation.w = 1;
	scanHeader_w.pose.rotation.x = 0;
	scanHeader_w.pose.rotation.y = 0;
	scanHeader_w.pose.rotation.z = 0;
	scanHeader_w.pose.translation.x = 0;
	scanHeader_w.pose.translation.y = 0;
	scanHeader_w.pose.translation.z = 0;

	scanHeader_w.pointFields.cartesianXField = true;
	scanHeader_w.pointFields.cartesianYField = true;
	scanHeader_w.pointFields.cartesianZField = true;

	scanHeader_w.pointFields.cartesianInvalidStateField = true;
	scanHeader_w.pointFields.colorRedField = true;
	scanHeader_w.pointFields.colorGreenField = true;
	scanHeader_w.pointFields.colorBlueField = true;
	scanHeader_w.pointFields.intensityField = true;
	


	scanHeader_w.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
	scanHeader_w.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
	scanHeader_w.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
	scanHeader_w.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
	scanHeader_w.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
	scanHeader_w.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;

	scanHeader_w.intensityLimits.intensityMinimum = 0.;
	scanHeader_w.intensityLimits.intensityMaximum = 1.;
	scanHeader_w.pointFields.intensityScaledInteger = 0.;

	scanIndex = eWriter.NewData3D(scanHeader_w);
	int64_t sizeChunks = 1024 * 1024 * 8;
	xData_w.resize(sizeChunks);
	yData_w.resize(sizeChunks);
	zData_w.resize(sizeChunks);
	redData_w.resize(sizeChunks);
	greenData_w.resize(sizeChunks);
	blueData_w.resize(sizeChunks);
	intData_w.resize(sizeChunks);
	_isInvalidData_w.resize(sizeChunks);


	
	dataWriter = std::make_unique<e57::CompressedVectorWriter>(eWriter.SetUpData3DPointsData(
		scanIndex,
		pointCount,
		xData_w.data(), yData_w.data(), zData_w.data(),
		_isInvalidData_w.data(),
		intData_w.data(),
		NULL,
		redData_w.data(), greenData_w.data(), blueData_w.data()
	));


	return 0;
}

//int SaveE57::writeHeader_v2() {
//		using namespace e57;
//
//		try {
//			/// Open new file for writing, get the initialized root node (a Structure).
//			/// Path name: "/"
//			
//			imf = new ImageFile(p_name, "w");
//			StructureNode root = imf->root();
//			imf->extensionsAdd("demo", "http://www.example.com/DemoExtension");
//			/// Set per-file properties.
//			/// Path names: "/formatName", "/majorVersion", "/minorVersion", "/coordinateMetadata"
//			root.set("formatName", StringNode(*imf, "ASTM E57 3D Imaging Data File"));
//			const e57::ustring globalGUID = createGUID();
//			//root.set("guid", StringNode(*imf, "3F2504E0-4F89-11D3-9A0C-0305E82C3310"));
//			root.set("guid", StringNode(*imf, globalGUID));
//			
//
//			/// Get ASTM version number supported by library, so can write it into file
//			int astmMajor;
//			int astmMinor;
//			ustring libraryId;
//			E57Utilities().getVersions(astmMajor, astmMinor, libraryId);
//			root.set("versionMajor", IntegerNode(*imf, astmMajor)); 
//			root.set("versionMinor", IntegerNode(*imf, astmMinor)); 
//			root.set("e57LibraryVersion", StringNode(*imf, "Octhread"));
//			/// Save a dummy string for coordinate system.
//			/// Really should be a valid WKT string identifying the coordinate reference system (CRS).
//			root.set("coordinateMetadata", StringNode(*imf, ""));
//
//
//			/// Create 3D data area.
//			/// Path name: "/data3D"
//			VectorNode data3D = VectorNode(*imf, true);
//			root.set("data3D", data3D);
//
//			/// Add first scan
//			/// Path name: "/data3D/0"
//			StructureNode scan0 = StructureNode(*imf);
//			data3D.append(scan0);
//
//			/// Add guid to scan0.
//			/// Path name: "/data3D/0/guid".
//			//const char* scanGuid0 = "3F2504E0-4F89-11D3-9A0C-0305E82C3310";
//			const e57::ustring scanGuid0 = createGUID();
//			scan0.set("guid", StringNode(*imf, scanGuid0));
//
//
//
//			///*Index bound*/
//			StructureNode indexBounds = StructureNode(*imf);
//
//			indexBounds.set("columnMaximum", IntegerNode(*imf, 1, std::numeric_limits<long long int>::min(), pointCount));
//			indexBounds.set("columnMinimum", IntegerNode(*imf, 0, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()));
//			indexBounds.set("rowMaximum", IntegerNode(*imf, pointCount, std::numeric_limits<long long int>::min(), pointCount));
//			indexBounds.set("rowMinimum", IntegerNode(*imf, 0, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()));
//			indexBounds.set("returnMinimum", IntegerNode(*imf, 0, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()));
//			indexBounds.set("returnMaximum", IntegerNode(*imf, 0, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()));
//			scan0.set("indexBounds", indexBounds);
//
//
//			/// Make a prototype of datatypes that will be stored in points record.
//			/// This prototype will be used in creating the points CompressedVector.
//			/// Using this proto in a CompressedVector will define path names like:
//			///      "/data3D/0/points/0/cartesianX"
//			StructureNode proto = StructureNode(*imf);
//			proto.set("cartesianX", FloatNode(*imf, 0.0, E57_DOUBLE, p_bb.min.x, p_bb.max.x));
//			proto.set("cartesianY", FloatNode(*imf, 0.0, E57_DOUBLE, p_bb.min.y, p_bb.max.y));
//			proto.set("cartesianZ", FloatNode(*imf, 0.0, E57_DOUBLE, p_bb.min.z, p_bb.max.z));
//			proto.set("cartesianInvalidState", IntegerNode(*imf, 0, 0, 2));
//			proto.set("intensity", FloatNode(*imf, 0.0, E57_SINGLE, 0.0, 1.0));
//			proto.set("colorRed", IntegerNode(*imf, 0, 0, 255));
//			proto.set("colorGreen", IntegerNode(*imf, 0, 0, 255));
//			proto.set("colorBlue", IntegerNode(*imf, 0, 0, 255));
//
//			/// Make empty codecs vector for use in creating points CompressedVector.
//			/// If this vector is empty, it is assumed that all fields will use the BitPack codec.
//			VectorNode codecs = VectorNode(*imf, true);
//
//			/// Create CompressedVector for storing points.  Path Name: "/data3D/0/points".
//			/// We use the prototype and empty codecs tree from above.
//			/// The CompressedVector will be filled by code below.
//			CompressedVectorNode *points = new CompressedVectorNode(*imf, proto, codecs);
//			scan0.set("points", *points);
//
//			/// Create pose structure for scan.
//			/// Path names: "/data3D/0/pose/rotation/w", etc...
//			///             "/data3D/0/pose/translation/x", etc...
//			StructureNode pose = StructureNode(*imf);
//			scan0.set("pose", pose);
//			StructureNode rotation = StructureNode(*imf);
//			pose.set("rotation", rotation);
//			rotation.set("w", FloatNode(*imf, 1.0));
//			rotation.set("x", FloatNode(*imf, 0.0));
//			rotation.set("y", FloatNode(*imf, 0.0));
//			rotation.set("z", FloatNode(*imf, 0.0));
//			StructureNode translation = StructureNode(*imf);
//			pose.set("translation", translation);
//			translation.set("x", FloatNode(*imf, 0.0));
//			translation.set("y", FloatNode(*imf, 0.0));
//			translation.set("z", FloatNode(*imf, 0.0));
//
//			///================
//			/// Add grouping scheme area
//			/// Path name: "/data3D/0/pointGroupingSchemes"
//			StructureNode pointGroupingSchemes = StructureNode(*imf);
//			scan0.set("pointGroupingSchemes", pointGroupingSchemes);
//
//			/// Add a line grouping scheme
//			/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine"
//			StructureNode groupingByLine = StructureNode(*imf);
//			groupingByLine.set("groupsSize", IntegerNode(*imf, 1)); //A enelever mayba 
//			groupingByLine.set("pointCountSize", IntegerNode(*imf, pointCount)); //A enlever Mayba
//
//			/// Add idElementName to groupingByLine, specify a line is column oriented
//			/// 
//			/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine/idElementName"
//			groupingByLine.set("idElementName", StringNode(*imf, "columnIndex"));
//			pointGroupingSchemes.set("groupingByLine", groupingByLine);
//			
//
//			/// Make a prototype of datatypes that will be stored in LineGroupRecord.
//			/// This prototype will be used in creating the groups CompressedVector.
//			/// Will define path names like:
//			///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/idElementValue"
//			StructureNode lineGroupProto = StructureNode(*imf);
//			lineGroupProto.set("idElementValue", IntegerNode(*imf, 0, 0/*, 4*/));
//			lineGroupProto.set("startPointIndex", IntegerNode(*imf, 0, 0/*, 4 * pointCount*/));
//			lineGroupProto.set("pointCount", IntegerNode(*imf, 1, 0/*, pointCount * 2*/));
//
//			/// Add cartesian bounds to line group prototype
//			/// Will define path names like:
//			///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/cartesianBounds/xMinimum"
//			StructureNode lineGroupBbox = StructureNode(*imf);
//			lineGroupProto.set("cartesianBounds", lineGroupBbox);
//			lineGroupBbox.set("xMinimum", FloatNode(*imf, 0.0));
//			lineGroupBbox.set("xMaximum", FloatNode(*imf, 0.0));
//			lineGroupBbox.set("yMinimum", FloatNode(*imf, 0.0));
//			lineGroupBbox.set("yMaximum", FloatNode(*imf, 0.0));
//			lineGroupBbox.set("zMinimum", FloatNode(*imf, 0.0));
//			lineGroupBbox.set("zMaximum", FloatNode(*imf, 0.0));
//
//			/// Make empty codecs vector for use in creating groups CompressedVector.
//			/// If this vector is empty, it is assumed that all fields will use the BitPack codec.
//			VectorNode lineGroupCodecs = VectorNode(*imf, true);
//
//			groups = new CompressedVectorNode(*imf, lineGroupProto, lineGroupCodecs);
//			groupingByLine.set("groups", *groups);
//			/// Create CompressedVector for storing groups.  
//			/// Path Name: "/data3D/0/pointGroupingSchemes/groupingByLine/groups".
//			/// We use the prototype and empty codecs tree from above.
//			/// The CompressedVector will be filled by code below.
//
//
//
//			/// Add Cartesian bounding box to scan.
//			/// Path names: "/data3D/0/cartesianBounds/xMinimum", etc...
//			StructureNode bbox = StructureNode(*imf);
//			bbox.set("xMinimum", FloatNode(*imf, -1000.0)); 
//			bbox.set("xMaximum", FloatNode(*imf, 1000.0));
//			bbox.set("yMinimum", FloatNode(*imf, -1000.0));
//			bbox.set("yMaximum", FloatNode(*imf, 1000.0));
//			bbox.set("zMinimum", FloatNode(*imf, -1000.0));
//			bbox.set("zMaximum", FloatNode(*imf, 1000.0));
//			scan0.set("cartesianBounds", bbox);
//
//
//			///================
//			/// Add name and description to scan
//			/// Path names: "/data3D/0/name", "/data3D/0/description".
//			// Changer le nom !!
//
//			boost::filesystem::path p(p_name);
//			std::string scanName = p.stem().string();
//
//			scan0.set("name", StringNode(*imf, scanName));
//			scan0.set("description", StringNode(*imf, ""));
//
//
//
//			///================VectorNode images2D = VectorNode(*imf, true);
//
//			VectorNode images2D = VectorNode(*imf, true);
//			root.set("images2D", images2D);
//
//			datas.xData.resize(writerChunckSize);
//			datas.yData.resize(writerChunckSize);
//			datas.zData.resize(writerChunckSize);
//			datas.isInvalidData.resize(writerChunckSize);
//			datas.intData.resize(writerChunckSize);
//			datas.redData.resize(writerChunckSize);
//			datas.greenData.resize(writerChunckSize);
//			datas.blueData.resize(writerChunckSize);
//
//			vector<SourceDestBuffer> sourceBuffers;
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianX", datas.xData.data(), writerChunckSize, true, true));
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianY", datas.yData.data(), writerChunckSize, true, true));
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianZ", datas.zData.data(), writerChunckSize, true, true));
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianInvalidState", datas.isInvalidData.data(), writerChunckSize, true));
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "intensity", datas.intData.data(), writerChunckSize, true));
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "colorRed", datas.redData.data(), writerChunckSize, true));
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "colorGreen", datas.greenData.data(), writerChunckSize, true));
//			sourceBuffers.push_back(SourceDestBuffer(*imf, "colorBlue", datas.blueData.data(), writerChunckSize, true));
//			num_max = 0;
//
//
//			writer = new CompressedVectorWriter(points->writer(sourceBuffers));
//		}
//
//		catch (E57Exception& ex) {
//			ex.report(__FILE__, __LINE__, __FUNCTION__);
//		}
//		catch (std::exception& ex) {
//			cerr << "Got an std::exception, what=" << ex.what() << endl;
//		}
//		catch (...) {
//			cerr << "Got an unknown exception" << endl;
//		}
//
//return 1;
//}

int SaveE57::write(std::vector<mypt3d>& pts) {
	//int64_t sizeChunks = 1024 * 1024 * 8;
	//std::vector<double> xData_w(sizeChunks);
	//std::vector<double> yData_w(sizeChunks);
	//std::vector<double> zData_w(sizeChunks);
	//std::vector<uint16_t> redData_w(sizeChunks);
	//std::vector<uint16_t> greenData_w(sizeChunks);
	//std::vector<uint16_t> blueData_w(sizeChunks);
	//std::vector<double> intData_w(sizeChunks);
	//std::vector<int8_t> _isInvalidData_w(sizeChunks);
	//e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
	//	scanIndex,
	//	pointCount,
	//	xData_w.data(), yData_w.data(), zData_w.data(),
	//	_isInvalidData_w.data(),
	//	intData_w.data(),
	//	NULL,
	//	redData_w.data(), greenData_w.data(), blueData_w.data()
	//);
	long int count = 0;
	while (count < pts.size()) {
		int i = 0;
		while (i < writerChunckSize && count < pts.size()) {
			xData_w[i] = pts[count].x;
			yData_w[i] = pts[count].y;
			zData_w[i] = pts[count].z;
			redData_w[i] = pts[count].r;
			greenData_w[i] = pts[count].g;
			blueData_w[i] = pts[count].b;
			intData_w[i] = pts[count].intensity;
			_isInvalidData_w[i] = 0;
			count++;
			++i;
		}
		if (i != 0) {
			dataWriter->write(i);
		}
	}
	num_max += count;
	return 0;
}


//write va écrire bloc par bloc dans le fichier
//int SaveE57::write(std::vector<mypt3d>& pts) 
//{
//	try {
//
//		long int count = 0;
//		while (count < pts.size()) {
//			int i = 0;
//			while (i < writerChunckSize && count < pts.size()) {
//				datas.xData[i] = pts[count].x;
//				datas.yData[i] = pts[count].y;
//				datas.zData[i] = pts[count].z;
//				datas.redData[i] = pts[count].r;
//				datas.greenData[i] = pts[count].g;
//				datas.blueData[i] = pts[count].b;
//				datas.intData[i] = pts[count].intensity;
//				datas.isInvalidData[i] = 0;
//				count++;
//				++i;
//			}
//			if (i != 0) {
//				writer->write(i);
//			}
//		}
//		num_max += count;
//
//		return 1;
//	}
//	catch (e57::E57Exception& ex) {
//		ex.report(__FILE__, __LINE__, __FUNCTION__);
//	}
//	catch (std::exception& ex) {
//		cerr << "Got an std::exception, what=" << ex.what() << endl;
//	}
//	catch (...) {
//		cerr << "Got an unknown exception" << endl;
//	}
//
//	return 0;
//}

int SaveE57::writeFooter() {

	dataWriter->close();
	dataWriter.reset();
	std::vector<int64_t> idElementValue_w;
	std::vector<int64_t> startPointIndex_w;
	vector<int64_t> pointCount_w;
	int group = 0;
	pointCount_w.push_back(num_max);
	idElementValue_w.push_back(0);
	startPointIndex_w.push_back(0);


	eWriter.WriteData3DGroupsData(scanIndex, 1,
		&idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);

	eWriter.Close();
	
	return 0;
}
//int SaveE57::writeFooter()
//{
//	try {
//		writer->close();
//
//		const int NG = 1;
//		int32_t idElementValue[NG] = { 0 };
//		int32_t startPointIndex[NG] = { 0 };
//		int64_t pointCount[NG] = { (int64_t)num_max };
//		//double  xMinimum[NG] = { 0 };
//		//double  xMaximum[NG] = { 1000, };
//		//double  yMinimum[NG] = { 0 };
//		//double  yMaximum[NG] = { 2000 };
//		//double  zMinimum[NG] = { 0 };
//		//double  zMaximum[NG] = { 2000 };
//		double  xMinimum[NG] = { std::numeric_limits<double>::min() };
//		double  xMaximum[NG] = { std::numeric_limits<double>::max() };
//		double  yMinimum[NG] = { std::numeric_limits<double>::min() };
//		double  yMaximum[NG] = { std::numeric_limits<double>::max() };
//		double  zMinimum[NG] = { std::numeric_limits<double>::min() };
//		double  zMaximum[NG] = { std::numeric_limits<double>::max() };
//
//		vector<e57::SourceDestBuffer> groupSDBuffers;
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "idElementValue", idElementValue, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "startPointIndex", startPointIndex, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "pointCount", pointCount, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/xMinimum", xMinimum, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/xMaximum", xMaximum, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/yMinimum", yMinimum, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/yMaximum", yMaximum, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/zMinimum", zMinimum, NG, true));
//		groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/zMaximum", zMaximum, NG, true));
//
//		/// Write source buffers into CompressedVector
//		{
//			e57::CompressedVectorWriter writer = groups->writer(groupSDBuffers);
//			writer.write(NG);
//			writer.close();
//		}
//		
//		imf->close();
//		delete groups;
//		delete imf;
//		delete writer;
//		return 1;
//		
//	}
//	catch (e57::E57Exception& ex) {
//		ex.report(__FILE__, __LINE__, __FUNCTION__);
//	}
//	catch (std::exception& ex) {
//		cerr << "Got an std::exception, what=" << ex.what() << endl;
//	}
//	catch (...) {
//		cerr << "Got an unknown exception" << endl;
//	}
//	return 0;
//}
