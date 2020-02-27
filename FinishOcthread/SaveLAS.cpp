#include "SaveLAS.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <laszip.hpp>

SaveLAS::SaveLAS(std::string _filename, unsigned long long int _numMax, BoundingBox& _bb) : SavableFile(_filename, _numMax, _bb)
{
	m_numEffective = 0;
	writeHeader();
}


SaveLAS::~SaveLAS()
{
}

int SaveLAS::write(std::vector<mypt3d>& pts) {
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

	return 0;
}

int SaveLAS::writeHeader() {
	LASwriteOpener laswriteopener;
	
	laswriteopener.set_file_name(p_name.c_str());
	lasheader.x_scale_factor = 0.001;
	lasheader.y_scale_factor = 0.001;
	lasheader.z_scale_factor = 0.001;
	lasheader.point_data_format = 2;
	lasheader.point_data_record_length = 28;
	laswriter = laswriteopener.open(&lasheader);
	if (laswriter == 0)
	{
		fprintf(stderr, "ERROR: could not open laswriter\n");

		return 1;
	}
	return 0;
}

int SaveLAS::writeFooter() {

	laswriter->update_header(&lasheader, TRUE);
	I64 total_bytes = laswriter->close();

	delete laswriter;
	return 0;
}
