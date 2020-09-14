#include "Part.h"
#include "../Octhreade57/E57Simple.h"
#include "../Octhreade57/E57Foundation.h"
#include <laszip.hpp>
#include "laswriter.hpp"
#include "lasreader.hpp"
struct e57WriteData {
	std::vector<double> xData, yData, zData;
	std::vector<double> isInvalidData;
	std::vector<double> intData;
	std::vector<int> redData, greenData, blueData;

	e57WriteData(long int num = 0) {
		xData.resize(num); yData.resize(num); zData.resize(num);
		isInvalidData.resize(num); intData.resize(num);
		redData.resize(num); greenData.resize(num); blueData.resize(num);
	}
};


Part::Part()
{
}


Part::~Part()
{
}

Part::Part(std::string _base, unsigned _x, unsigned _y, unsigned _z, BoundingBox _bb, float _sizeX, float _sizeY, float _sizeZ, pt3d& _offset) :
	m_x(_x), m_y(_y), m_z(_z),
	m_size_X(_sizeX), m_size_Y(_sizeY), m_size_Z(_sizeZ),
	m_bb(_bb), offset(_offset)
{
	
	m_file = MyFile(getName(_base));
	m_file.createFile();
	m_numPts = 0;
}

std::string Part::getName(std::string base)
{
	std::string name = base + "\\" + std::to_string(m_x) + "-" + std::to_string(m_y) + "-" + std::to_string(m_z);
	return name;
}

void Part::savePart(std::string baseCutter, std::string baseTemp, std::string ext) {
	std::string name = getName(baseCutter);
	//Ecire dans un fichier save

	std::vector<mypt3d> pts = getPtsFromFile();
	saveFile2(name, ext, pts);
}

void Part::update(std::string base) {
	std::string nameSave = base + "\\.Save";
	unsigned long numPointsTotal = m_pts.size() + m_numPts;
	if (numPointsTotal < 1024) {
		remove(getName(base).c_str());
	}
	else {
		std::ofstream file(nameSave, std::ios::out | std::ios::app);
		file << std::to_string(m_x) << std::to_string(m_y) << std::to_string(m_z) << " " <<
			numPointsTotal << " " <<
			m_bb.min.x << " " << m_bb.min.y << " " << m_bb.min.z << " " <<
			m_bb.max.x << " " << m_bb.max.y << " " << m_bb.max.z << "\n";
		file.close();
	}
}

bool Part::isIn(mypt3d& pt) {
	bool ret = (pt.x >= m_bb.min.x && pt.x < m_bb.max.x &&
		pt.y >= m_bb.min.y && pt.y < m_bb.max.y &&
		pt.z >= m_bb.min.z && pt.z < m_bb.max.z);


	return ret;
}

bool Part::isIn(mypt3d& pt, float overlap) {
	bool ret = 
		(
		pt.x >= (m_bb.min.x - overlap) && pt.x < (m_bb.max.x + overlap) &&
		pt.y >= (m_bb.min.y - overlap) && pt.y < (m_bb.max.y + overlap) &&
		pt.z >= (m_bb.min.z - overlap) && pt.z < (m_bb.max.z + overlap)
		);


	return ret;

}
void Part::addPoint(mypt3d& pt) {
	
	m_pts.push_back(pt);
}


void Part::writeInFile() {
	m_numPts += m_pts.size();
	m_file.writeToFile(m_pts);
	m_pts.clear();
	m_pts.resize(0);
	m_pts.shrink_to_fit();
}

int Part::size()
{
	return m_pts.size();
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

//const e57::ustring createGUID() {
//	std::string str = "ABCDEF0123456789";
//	std::string ret = "";
//	std::random_device rd{};
//	std::mt19937 engine{ rd() };
//
//	std::uniform_int_distribution<int>dist{ 0, (int)str.length() - 1 };
//	for (int i = 0; i < 36; ++i) {
//		if (i == 8 || i == 13 || i == 18 || i == 23) {
//			ret += "-";
//		}
//		else {
//			int rnd = dist(engine);
//			ret += str[rnd];
//		}
//	}
//	return ret;
//}

void Part::saveFile2(std::string name, std::string ext, std::vector<mypt3d>& pts) {
	if (m_numPts > 1024) {
		if (ext == ".e57") {
			std::string final_name = name + ext;
			e57::Writer eWriter(final_name, "");
			e57::Data3D scanHeader_w;

			scanHeader_w.name = final_name;

			int64_t pointCount = pts.size();

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

			pt3d center = m_bb.getCenter();
			center = center + offset;
			scanHeader_w.pose.rotation.w = 1;
			scanHeader_w.pose.rotation.x = 0;
			scanHeader_w.pose.rotation.y = 0;
			scanHeader_w.pose.rotation.z = 0;
			scanHeader_w.pose.translation.x = center.x;
			scanHeader_w.pose.translation.y = center.y;
			scanHeader_w.pose.translation.z = center.z;

			scanHeader_w.pointFields.cartesianXField = true;
			scanHeader_w.pointFields.cartesianYField = true;
			scanHeader_w.pointFields.cartesianZField = true;

			scanHeader_w.pointFields.cartesianInvalidStateField = true;
			scanHeader_w.pointFields.colorRedField = true;
			scanHeader_w.pointFields.colorGreenField = true;
			scanHeader_w.pointFields.colorBlueField = true;
			scanHeader_w.pointFields.intensityField = true;

			int64_t sizeChunks = 1024 * 1024 * 8;
			std::vector<double> xData_w(pointCount);
			std::vector<double> yData_w(pointCount);
			std::vector<double> zData_w(pointCount);
			std::vector<uint16_t> redData_w(pointCount);
			std::vector<uint16_t> greenData_w(pointCount);
			std::vector<uint16_t> blueData_w(pointCount);
			std::vector<double> intData_w(pointCount);
			std::vector<int8_t> _isInvalidData_w(pointCount);

			scanHeader_w.colorLimits.colorRedMinimum = e57::E57_UINT8_MIN;
			scanHeader_w.colorLimits.colorRedMaximum = e57::E57_UINT8_MAX;
			scanHeader_w.colorLimits.colorGreenMinimum = e57::E57_UINT8_MIN;
			scanHeader_w.colorLimits.colorGreenMaximum = e57::E57_UINT8_MAX;
			scanHeader_w.colorLimits.colorBlueMinimum = e57::E57_UINT8_MIN;
			scanHeader_w.colorLimits.colorBlueMaximum = e57::E57_UINT8_MAX;

			scanHeader_w.intensityLimits.intensityMinimum = 0.;
			scanHeader_w.intensityLimits.intensityMaximum = 1.;
			scanHeader_w.pointFields.intensityScaledInteger = 0.;
			
			int scanIndex = eWriter.NewData3D(scanHeader_w);

			e57::CompressedVectorWriter dataWriter = eWriter.SetUpData3DPointsData(
				scanIndex,
				pointCount,
				xData_w.data(), yData_w.data(), zData_w.data(),
				_isInvalidData_w.data(),
				intData_w.data(),
				NULL,
				redData_w.data(), greenData_w.data(), blueData_w.data()
			);

			
			for (int i = 0; i < pts.size(); ++i) {
				xData_w[i] = pts[i].x - center.x;
				yData_w[i] = pts[i].y - center.y;
				zData_w[i] = pts[i].z - center.z;

				redData_w[i] = pts[i].r;
				greenData_w[i] = pts[i].g;
				blueData_w[i] = pts[i].b;

				intData_w[i] = pts[i].intensity;
				_isInvalidData_w[i] = 0;
			}
			dataWriter.write(pointCount);

			std::vector<int64_t> idElementValue_w;
			std::vector<int64_t> startPointIndex_w;
			vector<int64_t> pointCount_w;
			int group = 0;
			dataWriter.close();
			pointCount_w.push_back(pointCount);
			idElementValue_w.push_back(0);
			startPointIndex_w.push_back(0);


			eWriter.WriteData3DGroupsData(scanIndex, 1,
				&idElementValue_w[0], &startPointIndex_w[0], &pointCount_w[0]);

			eWriter.Close();
		}
		if (ext == ".pts") {
			std::string final_name = name + ext;

			std::ofstream file(final_name, std::ios::out);

			file << pts.size() << "\n";
			for (int idx = 0; idx < pts.size(); ++idx) {
				file << pts[idx].x << " " << pts[idx].y << " " << pts[idx].z << " " << pts[idx].intensity << " " << (unsigned)pts[idx].r << " " << (unsigned)pts[idx].g << " " << (unsigned)pts[idx].b << "\n";
			}
			file.close();
		}
		if (ext == ".las" || ext == ".laz") {
			std::string final_name = name + ext;
			LASwriteOpener laswriteopener;
			LASheader lasheader;
			LASwriter* laswriter;
			laswriteopener.set_file_name(final_name.c_str());
			lasheader.x_scale_factor = 0.001;
			lasheader.y_scale_factor = 0.001;
			lasheader.z_scale_factor = 0.001;
			lasheader.point_data_format = 2;
			lasheader.point_data_record_length = 28;
			laswriter = laswriteopener.open(&lasheader);
			if (laswriter == 0)
			{
				fprintf(stderr, "ERROR: could not open laswriter\n");

				return;
			}

			LASpoint laspoint;

			laspoint.init(&lasheader, lasheader.point_data_format, lasheader.point_data_record_length, 0);
			for (int i = 0; i < pts.size(); ++i) {
				laspoint.set_x(pts[i].x);
				laspoint.set_y(pts[i].y);
				laspoint.set_z(pts[i].z);
				float intens_ = pts[i].intensity * 65536;
				U16 intens = (int)intens_;
				laspoint.set_intensity(intens);
				laspoint.set_R(pts[i].r);
				laspoint.set_G(pts[i].g);
				laspoint.set_B(pts[i].b);

				laswriter->write_point(&laspoint);
				laswriter->update_inventory(&laspoint);
			}

			laswriter->update_header(&lasheader, TRUE);
			I64 total_bytes = laswriter->close();

			delete laswriter;
			return;
		}
		if (ext == "") {

		}
	}
}

//Not used anymore
//void Part::saveFile(std::string name, std::string ext, std::vector<mypt3d>& pts) {
//	if (m_numPts > 1024) {
//		if (ext == ".e57") {
//			std::string final_name = name + ext;
//			using namespace e57;
//
//			try {
//				ImageFile imf = ImageFile(final_name, "w");
//				StructureNode root = imf.root();
//
//				root.set("formatName", StringNode(imf, "ASTM E57 3D Imaging Data File"));
//				const e57::ustring globalGUID = createGUID();
//				root.set("guid", StringNode(imf, globalGUID));
//
//				int astmMajor;
//				int astmMinor;
//				ustring libraryId;
//				E57Utilities().getVersions(astmMajor, astmMinor, libraryId);
//				root.set("versionMajor", IntegerNode(imf, astmMajor));
//				root.set("versionMinor", IntegerNode(imf, astmMinor));
//				root.set("e57LibraryVersion", StringNode(imf, ""));
//				root.set("coordinateMetadata", StringNode(imf, ""));
//				
//				VectorNode data3D = VectorNode(imf, true);
//				root.set("data3D", data3D);
//
//				StructureNode scan0 = StructureNode(imf);
//				data3D.append(scan0);
//
//				const e57::ustring scanGuid0 = createGUID();
//				scan0.set("guid", StringNode(imf, scanGuid0));
///*
//				scan0.set("collectionStartDateTime", FloatNode(imf, 0.0));
//				scan0.set("collectionEndDateTime", FloatNode(imf, 0.0));*/
//				StructureNode indexBounds = StructureNode(imf);
//
//				indexBounds.set("columnMaximum", IntegerNode(imf, 0/*1*//*, std::numeric_limits<long long int>::min(), m_numPts*/));
//				indexBounds.set("columnMinimum", IntegerNode(imf, 0/*, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()*/));
//				//indexBounds.set("rowMaximum", IntegerNode(imf, m_numPts, std::numeric_limits<long long int>::min()/*, m_numPts*/));
//				indexBounds.set("rowMinimum", IntegerNode(imf, 0/*, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()*/));
//				indexBounds.set("returnMinimum", IntegerNode(imf, 0/*, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()*/));
//				indexBounds.set("returnMaximum", IntegerNode(imf, 0/*, std::numeric_limits<long long int>::min(), std::numeric_limits<long long int>::max()*/));
//				scan0.set("indexBounds", indexBounds);
//
//				StructureNode creationDateTime = StructureNode(imf);
//				creationDateTime.set("dateTimeValue", FloatNode(imf, 0));
//				creationDateTime.set("isAtomicClockReferenced", IntegerNode(imf, 0));
//				root.set("creationDateTime", creationDateTime);
//
//				StructureNode proto = StructureNode(imf);
//				proto.set("cartesianX", FloatNode(imf, 0.0, E57_DOUBLE/*, m_bb.min.x, m_bb.max.x*/));
//				proto.set("cartesianY", FloatNode(imf, 0.0, E57_DOUBLE/*, m_bb.min.y, m_bb.max.y*/));
//				proto.set("cartesianZ", FloatNode(imf, 0.0, E57_DOUBLE/*, m_bb.min.z, m_bb.max.z*/));
//				proto.set("cartesianInvalidState", IntegerNode(imf, 0, 0, 2));
//				proto.set("intensity", FloatNode(imf, 0.0, E57_SINGLE, 0.0, 1.0));
//				proto.set("colorRed", IntegerNode(imf, 0, 0, 255));
//				proto.set("colorGreen", IntegerNode(imf, 0, 0, 255));
//				proto.set("colorBlue", IntegerNode(imf, 0, 0, 255));
//
//				VectorNode codecs = VectorNode(imf, true);
//
//				CompressedVectorNode *points = new CompressedVectorNode(imf, proto, codecs);
//				scan0.set("points", *points);
//
//				StructureNode pose = StructureNode(imf);
//				scan0.set("pose", pose);
//				StructureNode rotation = StructureNode(imf);
//				pose.set("rotation", rotation);
//				rotation.set("w", FloatNode(imf, 1.0));
//				rotation.set("x", FloatNode(imf, 0.0));
//				rotation.set("y", FloatNode(imf, 0.0));
//				rotation.set("z", FloatNode(imf, 0.0));
//				StructureNode translation = StructureNode(imf);
//				pose.set("translation", translation);
//				translation.set("x", FloatNode(imf, 0.0));
//				translation.set("y", FloatNode(imf, 0.0));
//				translation.set("z", FloatNode(imf, 0.0));
//
//				StructureNode pointGroupingSchemes = StructureNode(imf);
//				scan0.set("pointGroupingSchemes", pointGroupingSchemes);
//
//				/// Add a line grouping scheme
//				/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine"
//				StructureNode groupingByLine = StructureNode(imf);
//				//groupingByLine.set("groupsSize", IntegerNode(imf, 1)); //A enelever mayba 
//				//groupingByLine.set("pointCountSize", IntegerNode(imf, m_numPts)); //A enlever Mayba
//
//				/// Add idElementName to groupingByLine, specify a line is column oriented
//				/// 
//				/// Path name: "/data3D/0/pointGroupingSchemes/groupingByLine/idElementName"
//				groupingByLine.set("idElementName", StringNode(imf, "columnIndex"));
//				pointGroupingSchemes.set("groupingByLine", groupingByLine);
//
//
//				/// Make a prototype of datatypes that will be stored in LineGroupRecord.
//				/// This prototype will be used in creating the groups CompressedVector.
//				/// Will define path names like:
//				///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/idElementValue"
//				StructureNode lineGroupProto = StructureNode(imf);
//				lineGroupProto.set("idElementValue", IntegerNode(imf, 0, 0/*, 4*/));
//				lineGroupProto.set("startPointIndex", IntegerNode(imf, 0, 0/*, 4 * pointCount*/));
//				lineGroupProto.set("pointCount", IntegerNode(imf, 1, 0/*, pointCount * 2*/));
//
//				/// Add cartesian bounds to line group prototype
//				/// Will define path names like:
//				///     "/data3D/0/pointGroupingSchemes/groupingByLine/groups/0/cartesianBounds/xMinimum"
//				
//
//				StructureNode lineGroupBbox = StructureNode(imf);
//				lineGroupProto.set("cartesianBounds", lineGroupBbox);
//				lineGroupBbox.set("xMinimum", FloatNode(imf, 0.0));
//				lineGroupBbox.set("xMaximum", FloatNode(imf, 0.0));
//				lineGroupBbox.set("yMinimum", FloatNode(imf, 0.0));
//				lineGroupBbox.set("yMaximum", FloatNode(imf, 0.0));
//				lineGroupBbox.set("zMinimum", FloatNode(imf, 0.0));
//				lineGroupBbox.set("zMaximum", FloatNode(imf, 0.0));
//
//				/// Make empty codecs vector for use in creating groups CompressedVector.
//				/// If this vector is empty, it is assumed that all fields will use the BitPack codec.
//				VectorNode lineGroupCodecs = VectorNode(imf, true);
//
//				e57::CompressedVectorNode groups = CompressedVectorNode(imf, lineGroupProto, lineGroupCodecs);
//				groupingByLine.set("groups", groups);
//				/// Create CompressedVector for storing groups.  
//				/// Path Name: "/data3D/0/pointGroupingSchemes/groupingByLine/groups".
//				/// We use the prototype and empty codecs tree from above.
//				/// The CompressedVector will be filled by code below.
//
//
//
//				/// Add Cartesian bounding box to scan.
//				/// Path names: "/data3D/0/cartesianBounds/xMinimum", etc...
//				StructureNode bbox = StructureNode(imf);
//				//bbox.set("xMinimum", FloatNode(imf, m_bb.min.x));
//				//bbox.set("xMaximum", FloatNode(imf, m_bb.max.x));
//				//bbox.set("yMinimum", FloatNode(imf, m_bb.min.y));
//				//bbox.set("yMaximum", FloatNode(imf, m_bb.max.y));
//				//bbox.set("zMinimum", FloatNode(imf, m_bb.min.z));
//				//bbox.set("zMaximum", FloatNode(imf, m_bb.max.z));
//
//				bbox.set("xMinimum", FloatNode(imf, -10000.0));
//				bbox.set("xMaximum", FloatNode(imf, 10000.0));
//				bbox.set("yMinimum", FloatNode(imf, -10000.0));
//				bbox.set("yMaximum", FloatNode(imf, 10000.0));
//				bbox.set("zMinimum", FloatNode(imf, -10000.0));
//				bbox.set("zMaximum", FloatNode(imf, 10000.0));
//				scan0.set("cartesianBounds", bbox);
//
//
//				///================
//				/// Add name and description to scan
//				/// Path names: "/data3D/0/name", "/data3D/0/description".
//				// Changer le nom !!
//
//				boost::filesystem::path p(final_name);
//				std::string scanName = p.stem().string();
//
//				scan0.set("name", StringNode(imf, scanName));
//				scan0.set("description", StringNode(imf, ""));
//
//
//
//				///================VectorNode images2D = VectorNode(*imf, true);
//
//				VectorNode images2D = VectorNode(imf, true);
//				root.set("images2D", images2D);
//				unsigned long long 	writerChunckSize = 1024 * 1024 * 8;
//				e57WriteData datas;
//				datas.xData.resize(writerChunckSize);
//				datas.yData.resize(writerChunckSize);
//				datas.zData.resize(writerChunckSize);
//				datas.isInvalidData.resize(writerChunckSize);
//				datas.intData.resize(writerChunckSize);
//				datas.redData.resize(writerChunckSize);
//				datas.greenData.resize(writerChunckSize);
//				datas.blueData.resize(writerChunckSize);
//
//				vector<SourceDestBuffer> sourceBuffers;
//				sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianX", datas.xData.data(), writerChunckSize, true, true));
//				sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianY", datas.yData.data(), writerChunckSize, true, true));
//				sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianZ", datas.zData.data(), writerChunckSize, true, true));
//				sourceBuffers.push_back(SourceDestBuffer(imf, "cartesianInvalidState", datas.isInvalidData.data(), writerChunckSize, true));
//				sourceBuffers.push_back(SourceDestBuffer(imf, "intensity", datas.intData.data(), writerChunckSize, true));
//				sourceBuffers.push_back(SourceDestBuffer(imf, "colorRed", datas.redData.data(), writerChunckSize, true));
//				sourceBuffers.push_back(SourceDestBuffer(imf, "colorGreen", datas.greenData.data(), writerChunckSize, true));
//				sourceBuffers.push_back(SourceDestBuffer(imf, "colorBlue", datas.blueData.data(), writerChunckSize, true));
//
//
//
//				CompressedVectorWriter writer = CompressedVectorWriter(points->writer(sourceBuffers));
//
//				long int count = 0;
//				unsigned long long num_max = 0;
//				while (count < pts.size()) {
//					int i = 0;
//					while (i < writerChunckSize && count < pts.size()) {
//						datas.xData[i] = pts[count].x;
//						datas.yData[i] = pts[count].y;
//						datas.zData[i] = pts[count].z;
//						datas.redData[i] = pts[count].r;
//						datas.greenData[i] = pts[count].g;
//						datas.blueData[i] = pts[count].b;
//						datas.intData[i] = pts[count].intensity;
//						datas.isInvalidData[i] = 0;
//						count++;
//						++i;
//					}
//					if (i != 0) {
//						writer.write(i);
//					}
//				}
//				num_max += count;
//
//				writer.close();
//
//				const int NG = 1;
//				int32_t idElementValue[NG] = { 0 };
//				int32_t startPointIndex[NG] = { 0 };
//				int64_t pointCount[NG] = { (int64_t)num_max };
//				/*double  xMinimum[NG] = { 0 };
//				double  xMaximum[NG] = { 1000, };
//				double  yMinimum[NG] = { 0 };
//				double  yMaximum[NG] = { 2000 };
//				double  zMinimum[NG] = { 0 };
//				double  zMaximum[NG] = { 2000 };*/
//				double  xMinimum[NG] = { std::numeric_limits<double>::min() };
//				double  xMaximum[NG] = { std::numeric_limits<double>::max() };
//				double  yMinimum[NG] = { std::numeric_limits<double>::min() };
//				double  yMaximum[NG] = { std::numeric_limits<double>::max() };
//				double  zMinimum[NG] = { std::numeric_limits<double>::min() };
//				double  zMaximum[NG] = { std::numeric_limits<double>::max() };
//
//				vector<e57::SourceDestBuffer> groupSDBuffers;
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "idElementValue", idElementValue, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "startPointIndex", startPointIndex, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "pointCount", pointCount, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "cartesianBounds/xMinimum", xMinimum, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "cartesianBounds/xMaximum", xMaximum, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "cartesianBounds/yMinimum", yMinimum, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "cartesianBounds/yMaximum", yMaximum, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "cartesianBounds/zMinimum", zMinimum, NG, true));
//				groupSDBuffers.push_back(e57::SourceDestBuffer(imf, "cartesianBounds/zMaximum", zMaximum, NG, true));
//
//				/// Write source buffers into CompressedVector
//				{
//					e57::CompressedVectorWriter writer = groups.writer(groupSDBuffers);
//					writer.write(NG);
//					writer.close();
//				}
//				imf.close();
//			}
//
//
//
//			catch (E57Exception& ex) {
//				ex.report(__FILE__, __LINE__, __FUNCTION__);
//			}
//			catch (std::exception& ex) {
//				cerr << "Got an std::exception, what=" << ex.what() << endl;
//			}
//			catch (...) {
//				cerr << "Got an unknown exception" << endl;
//			}
//		}
//		if (ext == ".pts") {
//			std::string final_name = name + ext;
//			
//			std::ofstream file(final_name, std::ios::out);
//
//			file << pts.size() << "\n";
//			for (int idx = 0; idx < pts.size(); ++idx) {
//				file << pts[idx].x << " " << pts[idx].y << " " << pts[idx].z << " " <<pts[idx].intensity<< " "<< (unsigned)pts[idx].r << " " << (unsigned)pts[idx].g << " " << (unsigned)pts[idx].b << "\n";
//			}
//			file.close();
//		}
//		if (ext == ".las" ||ext == ".laz") {
//			std::string final_name = name + ext;
//			LASwriteOpener laswriteopener;
//			LASheader lasheader;
//			LASwriter* laswriter;
//			laswriteopener.set_file_name(final_name.c_str());
//			lasheader.x_scale_factor = 0.001;
//			lasheader.y_scale_factor = 0.001;
//			lasheader.z_scale_factor = 0.001;
//			lasheader.point_data_format = 2;
//			lasheader.point_data_record_length = 28;
//			laswriter = laswriteopener.open(&lasheader);
//			if (laswriter == 0)
//			{
//				fprintf(stderr, "ERROR: could not open laswriter\n");
//
//				return;
//			}
//
//			LASpoint laspoint;
//			
//			laspoint.init(&lasheader, lasheader.point_data_format, lasheader.point_data_record_length, 0);
//			for (int i = 0; i < pts.size(); ++i) {
//				laspoint.set_x(pts[i].x);
//				laspoint.set_y(pts[i].y);
//				laspoint.set_z(pts[i].z);
//				float intens_ = pts[i].intensity * 65536;
//				U16 intens = (int)intens_;
//				laspoint.set_intensity(intens);
//				laspoint.set_R(pts[i].r);
//				laspoint.set_G(pts[i].g);
//				laspoint.set_B(pts[i].b);
//
//				laswriter->write_point(&laspoint);
//				laswriter->update_inventory(&laspoint);
//			}
//
//			laswriter->update_header(&lasheader, TRUE);
//			I64 total_bytes = laswriter->close();
//
//			delete laswriter;
//			return;
//		}
//		if (ext == "") {
//
//		}
//	}
//}

std::vector<mypt3d> Part::getPtsFromFile()
{
	 std::vector<mypt3d> ret = m_file.readFromFile(m_numPts);
	 return ret;
}
