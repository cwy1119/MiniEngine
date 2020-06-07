#include "ScreenShot.h"

void ScreenShot(std::string filename, int width, int height) {
	FILE *fp = NULL;
	fp = fopen(filename.c_str(), "wb");
	int extra = (width * 3 + 3) / 4 * 4 - width * 3;
	BITMAPFILEHEADER fileheader = { 0 };
	fileheader.bfType = 0x4D42;
	fileheader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	fileheader.bfSize = fileheader.bfOffBits + height * (3 * width + extra);
	fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, fp);
	BITMAPINFOHEADER bmpheader = { 0 };
	bmpheader.biSize = sizeof(BITMAPINFOHEADER);
	bmpheader.biWidth = width;
	bmpheader.biHeight = height;
	bmpheader.biPlanes = 1;
	bmpheader.biBitCount = 24;
	fwrite(&bmpheader, sizeof(BITMAPINFOHEADER), 1, fp);
	int length = (width * 3 + extra)*height;
	GLubyte *data = new GLubyte[length];
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	fseek(fp, 0, SEEK_END);
	fwrite(data, length, 1, fp);
	fclose(fp);
	free(data);
}