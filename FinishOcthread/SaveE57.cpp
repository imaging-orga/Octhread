#include "SaveE57.h"




//On écrit dans un fichier auxiliaire avant de tout réécrire dans le fichier e57?
SaveE57::SaveE57(std::string _filename, long int _numMax, BoundingBox& bb) : SavableFile(_filename, _numMax, bb), eWriter(_filename, "")
{
	//Bien penser changer pointCount dans le loader
	pointCount = _numMax/* = 1024 * 512*/;
	writerChunckSize = std::min((long)1024*1024*8, _numMax / 3);
	writeHeader();
}

int SaveE57::writeTe(e57::CompressedVectorWriter* writer, int size_writer, std::vector<mypt3d>& pts,
	std::vector<double>& cartesianX, std::vector<double>& cartesianY, std::vector<double>& cartesianZ,
	std::vector<double>& cartesianInvalidState,
	std::vector<double>& intensity,
	std::vector<int>& red, std::vector<int>& green, std::vector<int>& blue) {
	long int count = 0;
	while (count < pts.size()) {
		int i = 0;
		while (i < size_writer && count < pts.size()) {
			cartesianX[i] = pts[count].x;
			cartesianY[i] = pts[count].y;
			cartesianZ[i] = pts[count].z;
			red[i] = pts[count].r/* - '0'*/;
			green[i] = pts[count].g/* - '0'*/;
			blue[i] = pts[count].b/* - '0'*/;
			intensity[i] = pts[count].intensity;
			cartesianInvalidState[i] = 0;
			count++;
			++i;
		}
		if (i != 0) {
			writer->write(i);
		}
	}
	return count;
}
int SaveE57::writeHeader() {
		using namespace e57;

		try {
			/// Open new file for writing, get the initialized root node (a Structure).
			/// Path name: "/"
			imf = new ImageFile(p_name, "w");
			StructureNode root = imf->root();
			imf->extensionsAdd("demo", "http://www.example.com/DemoExtension");
			/// Set per-file properties.
			/// Path names: "/formatName", "/majorVersion", "/minorVersion", "/coordinateMetadata"
			root.set("formatName", StringNode(*imf, "ASTM E57 3D Imaging Data File"));
			root.set("guid", StringNode(*imf, "3F2504E0-4F89-11D3-9A0C-0305E82C3300"));

			/// Get ASTM version number supported by library, so can write it into file
			int astmMajor;
			int astmMinor;
			ustring libraryId;
			E57Utilities().getVersions(astmMajor, astmMinor, libraryId);
			root.set("versionMajor", IntegerNode(*imf, astmMajor));
			root.set("versionMinor", IntegerNode(*imf, astmMinor));
			root.set("e57LibraryVersion", StringNode(*imf, "Octhread"));
			/// Save a dummy string for coordinate system.
			/// Really should be a valid WKT string identifying the coordinate reference system (CRS).
			root.set("coordinateMetadata", StringNode(*imf, ""));


			/// Create 3D data area.
			/// Path name: "/data3D"
			VectorNode data3D = VectorNode(*imf, true);
			root.set("data3D", data3D);

			/// Add first scan
			/// Path name: "/data3D/0"
			StructureNode scan0 = StructureNode(*imf);
			data3D.append(scan0);

			/// Add guid to scan0.
			/// Path name: "/data3D/0/guid".
			const char* scanGuid0 = "3F2504E0-4F89-11D3-9A0C-0305E82C3301";
			scan0.set("guid", StringNode(*imf, scanGuid0));



			///*Index bound*/
			StructureNode indexBounds = StructureNode(*imf);

			indexBounds.set("columnMaximum", IntegerNode(*imf, 1, 0, pointCount));
			indexBounds.set("columnMinimum", IntegerNode(*imf, 0, 0, 0));
			indexBounds.set("rowMaximum", IntegerNode(*imf, pointCount, 0, pointCount));
			indexBounds.set("rowMinimum", IntegerNode(*imf, 0, 0, 0));
			scan0.set("indexBounds", indexBounds);


			/// Make a prototype of datatypes that will be stored in points record.
			/// This prototype will be used in creating the points CompressedVector.
			/// Using this proto in a CompressedVector will define path names like:
			///      "/data3D/0/points/0/cartesianX"
			StructureNode proto = StructureNode(*imf);
			proto.set("cartesianX", FloatNode(*imf, 0.0, E57_DOUBLE, p_bb.min.x, p_bb.max.x));
			proto.set("cartesianY", FloatNode(*imf, 0.0, E57_DOUBLE, p_bb.min.y, p_bb.max.y));
			proto.set("cartesianZ", FloatNode(*imf, 0.0, E57_DOUBLE, p_bb.min.z, p_bb.max.z));
			proto.set("cartesianInvalidState", IntegerNode(*imf, 0, 0, 2));
			proto.set("intensity", FloatNode(*imf, 0.0, E57_SINGLE, 0.0, 1.0));
			proto.set("colorRed", IntegerNode(*imf, 0, 0, 255));
			proto.set("colorGreen", IntegerNode(*imf, 0, 0, 255));
			proto.set("colorBlue", IntegerNode(*imf, 0, 0, 255));

			/// Make empty codecs vector for use in creating points CompressedVector.
			/// If this vector is empty, it is assumed that all fields will use the BitPack codec.
			VectorNode codecs = VectorNode(*imf, true);

			/// Create CompressedVector for storing points.  Path Name: "/data3D/0/points".
			/// We use the prototype and empty codecs tree from above.
			/// The CompressedVector will be filled by code below.
			CompressedVectorNode *points = new CompressedVectorNode(*imf, proto, codecs);
			scan0.set("points", *points);

			/// Create pose structure for scan.
			/// Path names: "/data3D/0/pose/rotation/w", etc...
			///             "/data3D/0/pose/translation/x", etc...
			StructureNode pose = StructureNode(*imf);
			scan0.set("pose", pose);
			StructureNode rotation = StructureNode(*imf);
			pose.set("rotation", rotation);
			rotation.set("w", FloatNode(*imf, 1.0));
			rotation.set("x", FloatNode(*imf, 0.0));
			rotation.set("y", FloatNode(*imf, 0.0));
			rotation.set("z", FloatNode(*imf, 0.0));
			StructureNode translation = StructureNode(*imf);
			pose.set("translation", translation);
			translation.set("x", FloatNode(*imf, 0.0));
			translation.set("y", FloatNode(*imf, 0.0));
			translation.set("z", FloatNode(*imf, 0.0));

			///================
			/// Add grouping scheme area
			/// Path name: "/data3D/0/pointGroupingSchemes"
			StructureNode pointGroupingSchemes = StructureNode(*imf);
			scan0.set("pointGroupingSchemes", pointGroupingSchemes);

			/// Add a line grouping scheme
			/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine"
			StructureNode groupingByLine = StructureNode(*imf);
			pointGroupingSchemes.set("groupingByLine", groupingByLine);

			/// Add idElementName to groupingByLine, specify a line is column oriented
			/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine/idElementName"
			groupingByLine.set("idElementName", StringNode(*imf, "columnIndex"));

			/// Make a prototype of datatypes that will be stored in LineGroupRecord.
			/// This prototype will be used in creating the groups CompressedVector.
			/// Will define path names like:
			///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/idElementValue"
			StructureNode lineGroupProto = StructureNode(*imf);
			lineGroupProto.set("idElementValue", IntegerNode(*imf, 0, 0, 4));
			lineGroupProto.set("startPointIndex", IntegerNode(*imf, 0, 0/*, 4 * pointCount*/));
			lineGroupProto.set("pointCount", IntegerNode(*imf, 1, 0/*, pointCount * 2*/));

			/// Add cartesian bounds to line group prototype
			/// Will define path names like:
			///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/cartesianBounds/xMinimum"
			StructureNode lineGroupBbox = StructureNode(*imf);
			lineGroupProto.set("cartesianBounds", lineGroupBbox);
			lineGroupBbox.set("xMinimum", FloatNode(*imf, 0.0));
			lineGroupBbox.set("xMaximum", FloatNode(*imf, 0.0));
			lineGroupBbox.set("yMinimum", FloatNode(*imf, 0.0));
			lineGroupBbox.set("yMaximum", FloatNode(*imf, 0.0));
			lineGroupBbox.set("zMinimum", FloatNode(*imf, 0.0));
			lineGroupBbox.set("zMaximum", FloatNode(*imf, 0.0));

			/// Make empty codecs vector for use in creating groups CompressedVector.
			/// If this vector is empty, it is assumed that all fields will use the BitPack codec.
			VectorNode lineGroupCodecs = VectorNode(*imf, true);

			groups = new CompressedVectorNode(*imf, lineGroupProto, lineGroupCodecs);
			groupingByLine.set("groups", *groups);
			/// Create CompressedVector for storing groups.  
			/// Path Name: "/data3D/0/pointGroupingSchemes/groupingByLine/groups".
			/// We use the prototype and empty codecs tree from above.
			/// The CompressedVector will be filled by code below.



			/// Add Cartesian bounding box to scan.
	/// Path names: "/data3D/0/cartesianBounds/xMinimum", etc...
			StructureNode bbox = StructureNode(*imf);
			bbox.set("xMinimum", FloatNode(*imf, 0.0));
			bbox.set("xMaximum", FloatNode(*imf, 1000.0));
			bbox.set("yMinimum", FloatNode(*imf, 0.0));
			bbox.set("yMaximum", FloatNode(*imf, 1000.0));
			bbox.set("zMinimum", FloatNode(*imf, 0.0));
			bbox.set("zMaximum", FloatNode(*imf, 1000.0));
			scan0.set("cartesianBounds", bbox);
			///================
			/// Add name and description to scan
			/// Path names: "/data3D/0/name", "/data3D/0/description".
			scan0.set("name", StringNode(*imf, "MyTest"));
			scan0.set("description", StringNode(*imf, ""));



			///================VectorNode images2D = VectorNode(*imf, true);

			VectorNode images2D = VectorNode(*imf, true);
			root.set("images2D", images2D);

			datas.xData.resize(writerChunckSize);
			datas.yData.resize(writerChunckSize);
			datas.zData.resize(writerChunckSize);
			datas.isInvalidData.resize(writerChunckSize);
			datas.intData.resize(writerChunckSize);
			datas.redData.resize(writerChunckSize);
			datas.greenData.resize(writerChunckSize);
			datas.blueData.resize(writerChunckSize);

			vector<SourceDestBuffer> sourceBuffers;
			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianX", datas.xData.data(), writerChunckSize, true, true));
			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianY", datas.yData.data(), writerChunckSize, true, true));
			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianZ", datas.zData.data(), writerChunckSize, true, true));
			sourceBuffers.push_back(SourceDestBuffer(*imf, "cartesianInvalidState", datas.isInvalidData.data(), writerChunckSize, true));
			sourceBuffers.push_back(SourceDestBuffer(*imf, "intensity", datas.intData.data(), writerChunckSize, true));
			sourceBuffers.push_back(SourceDestBuffer(*imf, "colorRed", datas.redData.data(), writerChunckSize, true));
			sourceBuffers.push_back(SourceDestBuffer(*imf, "colorGreen", datas.greenData.data(), writerChunckSize, true));
			sourceBuffers.push_back(SourceDestBuffer(*imf, "colorBlue", datas.blueData.data(), writerChunckSize, true));
			num_max = 0;


			writer = new CompressedVectorWriter(points->writer(sourceBuffers));
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

return 1;
}





//write va écrire bloc par bloc dans le fichier
int SaveE57::write(std::vector<mypt3d>& pts) 
{
	if (pts.size() > 150)
		num_max += writeTe(writer, writerChunckSize, pts, datas.xData, datas.yData, datas.zData, datas.isInvalidData, datas.intData, datas.redData, datas.greenData, datas.blueData);
	return 1;
}





int SaveE57::writeFooter()
{
	writer->close();

	const int NG = 1;
	int32_t idElementValue[NG] = { 0 };
	int32_t startPointIndex[NG] = { 0 };
	int32_t pointCount[NG] = { num_max };
	double  xMinimum[NG] = { 0.0 };
	double  xMaximum[NG] = { 1000, };
	double  yMinimum[NG] = { 0 };
	double  yMaximum[NG] = { 1000 };
	double  zMinimum[NG] = { 0 };
	double  zMaximum[NG] = { 1000 };

	vector<e57::SourceDestBuffer> groupSDBuffers;
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "idElementValue", idElementValue, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "startPointIndex", startPointIndex, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "pointCount", pointCount, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/xMinimum", xMinimum, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/xMaximum", xMaximum, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/yMinimum", yMinimum, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/yMaximum", yMaximum, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/zMinimum", zMinimum, NG, true));
	groupSDBuffers.push_back(e57::SourceDestBuffer(*imf, "cartesianBounds/zMaximum", zMaximum, NG, true));

	/// Write source buffers into CompressedVector
	{
		e57::CompressedVectorWriter writer = groups->writer(groupSDBuffers);
		writer.write(NG);
		writer.close();
	}

	imf->close();
	delete groups;
	delete imf;
	delete writer;
	return 1;
}

SaveE57::~SaveE57()
{
}
