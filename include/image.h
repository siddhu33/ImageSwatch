#include <iostream>
#include <vector>
#include "color.h"

class Image{
	public:
	Image(std::string file_name){
		this->file_name = file_name;
	}
	void init();
	long unsigned int width = 0;
	long unsigned int height = 0;
	std::vector<std::vector<Color>> get_data() const {
		return data;
	}
	void write(std::vector<std::pair<int,int>> pairs, int top_hues, int swatch_height, std::string output_file = "output.png");
	private:
	void read_image(std::string ext);
	void read_png();
	void read_jpg();
	std::vector<std::string> extensions = { ".png", ".jpg", ".jpeg", ".PNG", ".JPG", ".JPEG" };
	std::string file_name = "";
	std::vector<std::vector<Color>> data;
};
