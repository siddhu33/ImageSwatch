#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include "image.h"


void fill_color_map(Image image, std::map<int, int> &color_map) {
	std::vector<std::vector<Color>> data = image.get_data();
	for (unsigned int i = 0; i < image.width; i++) {
		for (unsigned int j = 0; j < image.height; j++) {
			Color color = data[j][i];
			int hue = (int)color.get_hue();
			if(hue > 0){
				color_map[16 * (hue / 16)]++;
			}
		}
	}
}

/*
void save_output_bitmap(String ^ output_file, std::vector<std::pair<int,int>> pairs, int top_hues, int swatch_height) {
	int output_width = swatch_height * top_hues;
	int output_height = swatch_height;
	Bitmap output(output_width, output_height);
	for (int i = 0; i < output_width; i++) {
		for (int j = 0; j < output_height; j++) {
			output.SetPixel(i, j, color_from_hsv(1.0*pairs[i / swatch_height].first, 0.5, 0.75));
		}
	}
	output.Save(output_file);
}
*/
int main(int argc, char* argv[])
{
	if (argc < 2){
		std::cout << "Please add input file argument.\n";
		return 0;
	}
	std::string file_name = argv[1];
	std::map<int, int> color_map;
	std::cout << "reading from '" << file_name << "'\n";
	Image image(file_name);
	image.init();
	std::cout << "width : " << image.width << ", height : " << image.height << "\n";
	fill_color_map(image, color_map);
	std::vector<std::pair<int, int>> pairs;
	for (auto it = color_map.begin(); it != color_map.end(); ++it) {
		pairs.push_back(*it);
	}
	std::sort(pairs.begin(), pairs.end(), [=](std::pair<int, int> a, std::pair<int, int> b) { return a.second > b.second; });
	int top_hues = 10;
	int swatch_height = 500;
	image.write(pairs,top_hues,swatch_height, "out.png");
	return 0;
}
