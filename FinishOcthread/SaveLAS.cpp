#include "SaveLAS.h"



SaveLAS::SaveLAS(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb) : SavableFile(_filename, _numMax, _bb), m_stream(std::ofstream(_filename, std::ios::out | std::ios::binary))
{
	m_numEffective = 0;
	writeHeader();
}


SaveLAS::~SaveLAS()
{
}

int SaveLAS::write(std::vector<mypt3d>& pts) {
	for (int i = 0; i < pts.size(); ++i) {
		liblas::Point point(&m_header);
		point.SetCoordinates(pts[i].x, pts[i].y, pts[i].z);
		point.SetIntensity(pts[i].intensity);
		point.SetColor(liblas::Color(pts[i].r, pts[i].g, pts[i].b));
		m_writer->WritePoint(point);
	}
}

int SaveLAS::writeHeader() {
	
	m_header.SetDataFormatId(liblas::ePointFormat2); // X Y Z I R G B
	m_writer = new liblas::Writer(m_stream, m_header);
	return 1;
}

int SaveLAS::writeFooter() {
	m_stream.close();
	delete m_writer;
	return 1;
}
