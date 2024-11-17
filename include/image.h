#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "mainHeader.h"

// Placeholder class to make images easier to use

class image {
private:
	SDL_Surface* imageSurface;
	SDL_Texture* imageTexture;
	SDL_Rect imageRect;
public:
	
	bool visible;
	const char* filePath;

	image(const char* filePath, int width, int height, int locX, int locY):
		imageSurface(NULL),
		imageRect{locX, locY, width, height},
		visible(true),
		filePath(filePath)
	{}

	void initialize(SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer);

	void update(std::string filePath);

	bool isVisible() const;
};

class imageManager {
public:
	std::vector<image> images1d;

	std::vector<std::vector<image>> images2d;

	void add1d(image img);

	void add2d(image img, int row);

	void initAll1d(SDL_Renderer* renderer);

	void initAll2d(SDL_Renderer* renderer);

	void renderAll1d(SDL_Renderer *renderer);

	void renderAll2d(SDL_Renderer* renderer);

	/*
	Filled the area with sizeX x sizeY images with the size of width x height.
	*/
	void fill2d(int startX, int startY,
				int sizeX, int sizeY,
				int imgWidth, int imgHeight,
				const char* imgPath);
};

#endif /* IMAGE_H */
