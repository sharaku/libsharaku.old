/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. *
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <devices/color/simuration-color.hpp>

// bitmap画像を読み込むための定義
#define FILE_TYPE		0x4D42
#define FILE_HEADER_SIZE	14
#define INFO_HEADER_SIZE	40
#define DEFAULT_HEADER_SIZE	(FILE_HEADER_SIZE + INFO_HEADER_SIZE)

#pragma pack(2)
typedef struct BITMAPFILEHEADER {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} BITMAPFILEHEADER_t;
#pragma pack()
typedef struct BITMAPINFOHEADER {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAPINFOHEADER_t;

image_t *
read_bitmap_simple(const char *filename)
{
	int32_t x, y;
	int32_t width, height;
	int32_t stride;
	uint8_t *row, *buffer;
	image_t *result = NULL;
	int fd;
	int rc;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		goto out;
	}

	BITMAPFILEHEADER file;
	rc = read(fd, &file, sizeof file);
	if (rc < 0) {
		goto out;
	}
	BITMAPINFOHEADER info;
	rc = read(fd, &info, sizeof info);
	if (rc < 0) {
		goto out;
	}

	width = info.biWidth;
	height = info.biHeight;
	stride = (width * 3 + 3) / 4 * 4;
	result = (image_t *)malloc(sizeof(image_t) + width * height * sizeof(color_t));
	result->width = width;
	result->height = height;

	buffer = (uint8_t *)malloc(stride);
	for (y = height - 1; y >= 0; y--) {
		rc = read(fd, buffer, stride);
		if (rc < 0) {
			free(result);
			result = NULL;
			goto rele;
		}
		row = buffer;
		for (x = 0; x < width; x++) {
			result->map[y * width + x].b = *row++;
			result->map[y * width + x].g = *row++;
			result->map[y * width + x].r = *row++;
			result->map[y * width + x].a = 0xff;
		}
	}
rele:
	free(buffer);
	close(fd);
out:
	return result;
}

NAMESPACE_SHARAKU_BEGIN

simuration_color::simuration_color()
{
	_pxcel.set_color(0, 0, 0, 0);
	_ambient = 0;
	_reflected = 0;
	_img = NULL;
}

simuration_color::~simuration_color()
{
	if (_img) {
		free(_img);
		_img = NULL;
	}
}

void
simuration_color::set_imagefile(const char *filename)
{
	if (_img) {
		free(_img);
		_img = NULL;
	}
	_img = read_bitmap_simple(filename);
}


// 現在の位置を設定する。
// 地面との高さ、距離、角度は考慮しないものとする。
void
simuration_color::set_position(int32_t x, int32_t y)
{
	// メモリ上に読み込んだイメージのx,y座標の色を取得し、保存する。
	// 色を取得する際は保存した値を使用する。
	// 座標の単位はmm。
	if (_img) {
		_pxcel = _img->get_pixcel(x, y);
	}

	// 仮で環境光と反射光は同じとする。
	// 値はカラー値の平均(0-255)を使用する。
	// 白は71になるように制御する。
	// ((255 + 255 + 255) * 24) / 256
	_ambient = ((_pxcel.r + _pxcel.g + _pxcel.b) * 24) >> 8;
	_reflected = _ambient;
}

NAMESPACE_SHARAKU_END
