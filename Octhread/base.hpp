#pragma once

class pt3d {
public:
	float x, y, z;

	pt3d(float x_  = 0.f, float y_ = 0.f, float z_ = 0.f) 
		: x(x_), y(y_), z(z_) 
	{}

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