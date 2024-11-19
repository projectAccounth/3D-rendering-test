#include "../include/program.h"
#include "../include/mainHeader.h"
#include "../include/algorithms.h"

// Definition of the createRenderer() function
SDL_Renderer* Program::createRenderer(SDL_Window *targetWindow) {
	// The SDL_Renderer that will be returned
	return SDL_CreateRenderer(targetWindow, -1, SDL_RENDERER_PRESENTVSYNC);
}

SDL_Window* Program::createWindow(int height, int width, bool isFullScreen, const char* title) {
	// Create a window with the name of the passed argument title,
	// with the width and height of the passed arguments
	// and the window position at the center of the screen
	SDL_Window *targetWindow = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height, 0);
	// Checking whether the window was created or not
	if (targetWindow == NULL) {
		std::cout << ("Could not create window: %s\n", SDL_GetError());
		exit(1);
	}
	// Creating the renderer
	
	// If you want to go to fullscreen mode by default, pass "true" as
	// the argument to isFullScreen
	if (isFullScreen) {
		SDL_SetWindowFullscreen(targetWindow, SDL_WINDOW_FULLSCREEN);
	}
	return targetWindow;
}


void Program::onQuit(SDL_Window *window, SDL_Renderer *renderer) {
	// Destroy window and renderer
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	// Quit all SDL subsystems
	SDL_Quit();
}

void Program::programControls(EulerAngles& camRotation, Point3& position,
	Point3& displayPos, std::vector<Vector3>& d_vs,
	std::vector<Point3>& vertices, std::vector<Point2>& projectionPs,
	EulerAngles &boxRotation, imageManager &manager) {

	// Getting keyboardState
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);
	if (keyState[SDL_SCANCODE_A]) {
		resetAllImages(manager.images2d);
		// std::cout << "A\n";	
		movePointLeft(position, camRotation, 1);
		// std::cout << "Camera position: " << position.x << ' ' << position.y << '\n';
		updateAngles(camRotation, position, displayPos, d_vs, vertices, projectionPs);
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, position, displayPos, vertices, projectionPs);
		updateImage(vertices, projectionPs, manager, nearPlane, displayPos);
	}
	else if (keyState[SDL_SCANCODE_D]) {
		resetAllImages(manager.images2d);
		// std::cout << "D\n";
		movePointRight(position, camRotation, 1);
		// std::cout << "Camera position: " << position.x << ' ' << position.y << '\n';
		updateAngles(camRotation, position, displayPos, d_vs, vertices, projectionPs);
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, position, displayPos, vertices, projectionPs);
		updateImage(vertices, projectionPs, manager, nearPlane, displayPos);
	}
	else if (keyState[SDL_SCANCODE_S]) {
		resetAllImages(manager.images2d);
		// std::cout << "S\n";
		movePointBack(position, camRotation, 1);
		// std::cout << "Camera position: " << position.x << ' ' << position.y << '\n';
		updateAngles(camRotation, position, displayPos, d_vs, vertices, projectionPs);
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, position, displayPos, vertices, projectionPs);
		updateImage(vertices, projectionPs, manager, nearPlane, displayPos);
	}
	else if (keyState[SDL_SCANCODE_W]) {
		resetAllImages(manager.images2d);
		// std::cout << "W\n";
		movePointForward(position, camRotation, 1);
		// std::cout << "Camera position: " << position.x << ' ' << position.y << '\n';
		updateAngles(camRotation, position, displayPos, d_vs, vertices, projectionPs);
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, position, displayPos, vertices, projectionPs);
		updateImage(vertices, projectionPs, manager, nearPlane, displayPos);
	}
	else if (keyState[SDL_SCANCODE_E]) {
		resetAllImages(manager.images2d);
		// std::cout << "W\n";
		position.y += 1;
		// std::cout << "Camera position: " << position.x << ' ' << position.y << '\n';
		updateAngles(camRotation, position, displayPos, d_vs, vertices, projectionPs);
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, position, displayPos, vertices, projectionPs);
		updateImage(vertices, projectionPs, manager, nearPlane, displayPos);
	}
	else if (keyState[SDL_SCANCODE_Q]) {
		resetAllImages(manager.images2d);
		// std::cout << "W\n";
		position.y -= 1;
		// std::cout << "Camera position: " << position.x << ' ' << position.y << '\n';
		updateAngles(camRotation, position, displayPos, d_vs, vertices, projectionPs);
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, position, displayPos, vertices, projectionPs);
		updateImage(vertices, projectionPs, manager, nearPlane, displayPos);
	}
}

// Definition of the main loop function
bool Program::processEvent(SDL_Event event) {
	bool isRunning = true;

	switch (event.type) {
		case SDL_WINDOWEVENT_CLOSE: {
			// In case the program is closed manually
			isRunning = false;
			break;
		}
		case SDL_QUIT: {
			// In case SQL_Quit() is called manually
			isRunning = false;
			break;
		}
	}

	// returning the boolean
	return isRunning;
}