#include <stdint.h>
#include <cstdio>
//#include "utils.h"
#define STEG_HEADER_SIZE sizeof(uint32_t) * 8

enum ImageType {
	PNG, JPG, BMP, TGA
};

struct Image {
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels{4};

	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);

	~Image();

	bool read(const char* filename);
	bool write(const char* filename);
	void SetPixel(int i, Color3 c);

	ImageType get_file_type(const char* filename);
};