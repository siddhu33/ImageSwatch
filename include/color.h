#include <iostream>

class Color{
	public:
	Color(int r, int g, int b, int a){
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	static Color from_hsv(double hue, double saturation, double value);
	float get_hue() const;
	friend std::ostream& operator<< (std::ostream& stream, const Color color);
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 255;
};
