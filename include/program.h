#pragma once

// Include guard
#ifndef PROGRAM_H
#define PROGRAM_H

#include "mainHeader.h"
#include "vector.h"
#include "angles.h"
#include "image.h"

class Program {
public:
	// Creates the renderer
	SDL_Renderer* createRenderer(SDL_Window* targetWindow);

	// The function responsible for initializing the program and creating the window
	SDL_Window* createWindow(int height, int width, bool isFullScreen, const char* title);

	// Called on program exit
	void onQuit(SDL_Window* window, SDL_Renderer* renderer);

	// All program controls go here
	void programControls(EulerAngles& camRotation, Point3& position,
		Point3& displayPos, std::vector<Vector3>& d_vs,
		std::vector<Point3>& vertices, std::vector<Point2>& projectionPs,
		EulerAngles& boxRotation, imageManager& manager);

	bool processEvent(SDL_Event event);
};

#endif /* PROGRAM_H */
