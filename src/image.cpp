#include "../include/image.h"

void image::initialize(SDL_Renderer* renderer) {
	imageSurface = IMG_Load(filePath);
	if (imageSurface == nullptr) {
		std::cout << "A problem occurred when trying to create image. Error: " << SDL_GetError() << '\n';
		return;
	}
	imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);	
}

void image::render(SDL_Renderer* renderer) {
	if (!isVisible()) return;

	if (filePath != prevFilePath) {
		SDL_DestroyTexture(imageTexture);
		imageSurface = IMG_Load(filePath);
		imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
		SDL_FreeSurface(imageSurface);
		prevFilePath = filePath;
	}

	SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
}

void image::update(std::string str) {
	filePath = str.c_str();
}

bool image::isVisible() const { return visible; }

void imageManager::add1d(image img) {
	images1d.push_back(img);
}

void imageManager::renderAll1d(SDL_Renderer* renderer) {
	for (auto& img : images1d) {
		img.render(renderer);
	}
}

void imageManager::add2d(image img, int row) {
	images2d.resize(row);
	images2d[row].push_back(img);
}

void imageManager::renderAll2d(SDL_Renderer* renderer) {
	for (auto &imgs : images2d) {
		for (auto& img : imgs) {
			img.render(renderer);
		}
	}
}

void imageManager::initAll1d(SDL_Renderer* renderer) {
	for (auto& img : images1d) {
		img.initialize(renderer);
	}
}

void imageManager::initAll2d(SDL_Renderer* renderer) {
	for (auto& imgs : images2d) {
		for (auto& img : imgs) {
			img.initialize(renderer);
			// std::cout << "image at address " << &img << " initialized\n";
		}
	}
}

void imageManager::fill2d(int startX, int startY,
						  int sizeX, int sizeY,
						  int imgWidth, int imgHeight,
						  const char* imgPath) {
	images2d.resize(sizeY);
	for (int y = 0; y < sizeY; y++) {
		images2d[y].reserve(sizeX);
		for (int x = 0; x < sizeX; x++) {
			image img(imgPath, imgWidth, imgHeight, startX + imgWidth * x, startY + imgHeight * y);
			images2d[y].push_back(img);
		}
	}
}