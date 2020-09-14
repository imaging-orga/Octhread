#pragma once
#include "../e57/E57Simple.h"
#include "../e57/E57Foundation.h"

namespace OPTIMIZE {
	struct MyPoint {
	public:
		float x, y, z;
		float intensity;
		uint16_t r, g, b;

		MyPoint(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _intensity = 0.f, uint16_t _r = 0, uint16_t _g = 0, uint16_t _b = 0) :
			x(_x), y(_y), z(_z), intensity(_intensity), r(_r), g(_g), b(_b)
		{

		}
	};

	void process(std::string nameIn, std::string nameOut);
	int Multiple(std::string nameIn, std::string nameOut);
	int Unique(std::string nameIn, std::string nameOut);
}