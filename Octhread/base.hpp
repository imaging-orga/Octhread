#pragma once
#include <vector>
class pt3d {
public:
	float x, y, z;

	pt3d(float x_  = 0.f, float y_ = 0.f, float z_ = 0.f) 
		: x(x_), y(y_), z(z_) 
	{}


	pt3d operator+(pt3d& pt) {
		pt3d ret;
		ret.x = x + pt.x;
		ret.y = y + pt.y;
		ret.z = z + pt.z;
		return ret;
	}


	pt3d operator-(pt3d& pt) {
		pt3d ret;
		ret.x = x - pt.x;
		ret.y = y - pt.y;
		ret.z = z - pt.z;
		return ret;
	}

	pt3d operator/(float k) {
		pt3d ret;
		ret.x = x / k;
		ret.y = y / k;
		ret.z = z / k;
		return ret;
	}

	pt3d operator*(float k) {
		pt3d ret;
		ret.x = x * k;
		ret.y = y * k;
		ret.z = z * k;
		return ret;
	}

};

class mypt3d {
public:
	float x, y, z;
	float intens;
	int r, g, b;
	
	mypt3d(float x_ = 0.f, float y_ = 0.f, float z_ = 0.f, float intens_ = 0.f, int r_ = 0, int g_ = 0, int b_ = 0)
		: x(x_), y(y_), z(z_), intens(intens_), r(r_), g(g_), b(b_)
	{}
};


class BoundingBox {
public:
	pt3d min, max;

	BoundingBox(pt3d min_ = pt3d(0.f, 0.f, 0.f), pt3d max_ = pt3d(0.f, 0.f, 0.f)) 
		: min(min_), max(max_) 
	{}

	pt3d getCenter() {
		return (max + min) / 2.0;
	}

	pt3d getHalfDimension() {
		return (max - min) / 2.0;
	}
};
