#include <algorithm>
#include <fstream>
#include <exception>
#include "image.h"
#include "png.h"

//Thanks to 'niw' for their libpng code
//https://gist.github.com/niw/5963798

void Image::init(){
	int idx = file_name.rfind('.');
	std::string ext = file_name.substr(idx);
	if(std::find(extensions.begin(), extensions.end(), ext) != extensions.end()){
		this->read_image(ext);
	} else {
		std::cout << "extension " << ext << " is not valid\n";
	}
}

void Image::read_png(){
	FILE *fp = fopen(file_name.c_str(), "rb");
	int offset = 8;
	if(fp){
		char* header = new char[offset];
		int read = fread(header, 1, offset, fp);
		int is_png = !png_sig_cmp((png_bytep)header, 0, offset);
    		if (read != offset && !is_png){
			throw std::runtime_error(file_name + " is not an actual png file");
		}
		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png){
			throw std::runtime_error("Could not create png read struct");
		}
		png_infop info = png_create_info_struct(png);
		if (!info){
			png_destroy_read_struct(&png,(png_infopp)NULL, (png_infopp)NULL);
			throw std::runtime_error("Could not create png info struct");
		}

		png_init_io(png, fp);
		png_set_sig_bytes(png, offset);
		png_read_info(png, info);

		width      = png_get_image_width(png, info);
		height     = png_get_image_height(png, info);
		int color_type = png_get_color_type(png, info);
		int bit_depth  = png_get_bit_depth(png, info);

		if(bit_depth == 16){
			png_set_strip_16(png);
		}
		if(color_type == PNG_COLOR_TYPE_PALETTE){
			png_set_palette_to_rgb(png);
		}
		// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
		if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8){
			png_set_expand_gray_1_2_4_to_8(png);
		}
		if(png_get_valid(png, info, PNG_INFO_tRNS)){
			png_set_tRNS_to_alpha(png);
		}
		// These color_type don't have an alpha channel then fill it with 0xff.
		if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE){
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
		}
		if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
			png_set_gray_to_rgb(png);
		}

		png_read_update_info(png, info);

		//allocate memory for file read
  		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  		for(unsigned int y = 0; y < height; y++) {
    			row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  		}

		png_read_image(png, row_pointers);

		//load into class data attribute.
		for(unsigned int y = 0; y < height; y++) {
			png_byte* row = row_pointers[y];
			data.push_back(std::vector<Color>());
			for(unsigned int x = 0; x < width; x++){
				unsigned int idx = 4 * x;
				data[y].push_back(Color(row[idx], row[idx+1], row[idx+2], row[idx+3]));
			}
		}

		//free memory that was malloc'd and close file.
		for(unsigned int y = 0; y < height; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);
		fclose(fp);

	}else{
		std::cout << "file " << file_name << " is not valid.\n";
	}
}

void Image::write(std::vector<std::pair<int,int>> pairs, int top_hues, int swatch_height, std::string output_file){
	unsigned int output_width = swatch_height * top_hues;
	unsigned int output_height = swatch_height;

	std::cout << "writing to " << output_file << "\n";
	FILE *fp = fopen(output_file.c_str(), "wb");
	if(!fp){
		throw std::runtime_error("bad file");
	}
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) {
		throw std::runtime_error("cannot create write struct");
	}

	png_infop info = png_create_info_struct(png);
	if (!info) {
		throw std::runtime_error("cannot create info struct");
	}
	if (setjmp(png_jmpbuf(png))){
		throw std::runtime_error("error within setjmp command");
	}

	png_init_io(png, fp);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
	png,
	info,
	output_width, output_height,
	8,
	PNG_COLOR_TYPE_RGBA,
	PNG_INTERLACE_NONE,
	PNG_COMPRESSION_TYPE_DEFAULT,
	PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(png, info);

	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	//png_set_filler(png, 0, PNG_FILLER_AFTER);

	//allocate memory for writing to image
	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * output_height);
	for(unsigned int y = 0; y < output_height; y++) {
		row_pointers[y] = (png_byte*)malloc(sizeof(png_bytep)*4*output_width);
	}

	//fill 2D-array with image data for swatch.
	for (unsigned int i = 0; i < output_height; i++) {
		for (unsigned int j = 0; j < output_width; j++) {
			Color outputColor = Color::from_hsv(1.0*pairs[j / swatch_height].first, 1.0, 0.5);
			row_pointers[i][4*j] = outputColor.r;
			row_pointers[i][4*j+1] = outputColor.g;
			row_pointers[i][4*j+2] = outputColor.b;
			row_pointers[i][4*j+3] = outputColor.a;
		}
	}

	png_write_image(png, row_pointers);
	png_write_end(png, NULL);
	
	std::cout << "written output file" << std::endl;

	for(unsigned int y = 0; y < output_height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);

	fclose(fp);
}

void Image::read_jpg(){
	//TODO - jpg integration.
	return;
}

void Image::read_image(std::string ext){
	if(ext == ".png" || ext == ".PNG"){
		this->read_png();
	}
	else if(ext == ".jpg" || ext == ".jpeg" || ext == ".JPG" || ext == ".JPEG"){
		this->read_jpg();
	}
}
