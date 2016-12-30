#include "color.h"
#include <cmath>

Color Color::from_hsv(double hue, double saturation, double value){
	//http://stackoverflow.com/questions/359612/how-to-change-rgb-color-to-hsv
	int hi = (int)(std::floor(hue / 60)) % 6;
	double f = hue / 60 - std::floor(hue / 60);

	value = value * 255;
	int v = (int)(value);
	int p = (int)(value * (1 - saturation));
	int q = (int)(value * (1 - f * saturation));
	int t = (int)(value * (1 - (1 - f) * saturation));

	if (hi == 0)
		return Color(v, t, p, 255);
	else if (hi == 1)
		return Color(q, v, p, 255);
	else if (hi == 2)
		return Color(p, v, t, 255);
	else if (hi == 3)
		return Color(p, q, v, 255);
	else if (hi == 4)
		return Color(t, p, v, 255);
	else
		return Color(v, p, q, 255);
}

float Color::get_hue() const {
	float min = std::min(std::min(r, g), b);
	float max = std::max(std::max(r, g), b);
	if(max - min == 0){
		return -1.0f;
	}
	float hue = 0.0f;
	if (max == r) {
		hue = (g - b) / (max - min);

	} else if (max == g) {
		hue = 2.0f + (b - r) / (max - min);

	} else {
		hue = 4.0f + (r - g) / (max - min);
	}

	hue = hue * 60.0f;
	if (hue < 0.0f) hue = hue + 360.0f;
	return hue;	
}

std::ostream& operator<< (std::ostream& stream, const Color color){
	stream << "Color [" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "]\n";
	return stream;
}
