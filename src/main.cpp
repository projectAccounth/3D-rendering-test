// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/button.h"
#include "../include/textBox.h"
#include "../include/buttonManager.h"
#include "../include/image.h"
#include "../include/point.h"
#include "../include/angles.h"

// initialization
SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

// window's color, change please
SDL_Color windowColor = {255, 255, 255 ,255};

imageManager manager;
buttonManager btnManager;

Point3 camPos(0, 0, 0);
Point3 displayPos(0, 0, 10);

int tilesCountX = 32, tilesCountY = 32;

int main(int argc, char* argv[]) {

	manager.fill2d(0, 0, tilesCountX, tilesCountY, 15, 15, "./res/imgs/lamp.png");

	Program program;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	TTF_Font* mssnsf = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

	if (mssnsf == nullptr) {
		std::cout << "A problem occurred when creating font: " << TTF_GetError() << '\n';
	}

	mainWindow = program.createWindow(WINDOW_HEIGHT, WINDOW_WIDTH, 0, "Program");

	mainRenderer = program.createRenderer(mainWindow);

	int btnHeight = WINDOW_HEIGHT - 15 * tilesCountY;

	textButton goLeft(0 * WINDOW_WIDTH / 4, WINDOW_HEIGHT - btnHeight, WINDOW_WIDTH / 4, WINDOW_HEIGHT,
					SDL_Color(177, 177, 177, 255),
					"Left",
					SDL_Color(0, 0, 0, 255), SDL_Color(133, 133, 133, 199),
					mssnsf, CENTER, LEFT);
	textButton goRight(WINDOW_WIDTH / 4, WINDOW_HEIGHT - btnHeight, WINDOW_WIDTH / 4, WINDOW_HEIGHT,
					SDL_Color(177, 177, 177, 255),
					"Right",
					SDL_Color(0, 0, 0, 255), SDL_Color(133, 133, 133, 199),
					mssnsf, CENTER, LEFT);
	textButton goUp(2 * WINDOW_WIDTH / 4, WINDOW_HEIGHT - btnHeight, WINDOW_WIDTH / 4, WINDOW_HEIGHT,
					SDL_Color(177, 177, 177, 255),
					"Up",
					SDL_Color(0, 0, 0, 255), SDL_Color(133, 133, 133, 199),
					mssnsf, CENTER, LEFT);
	textButton goDown(3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT - btnHeight, WINDOW_WIDTH / 4, WINDOW_HEIGHT,
					SDL_Color(177, 177, 177, 255),
					"Down",
					SDL_Color(0, 0, 0, 255), SDL_Color(133, 133, 133, 199),
					mssnsf, CENTER, LEFT);

	btnManager.add(goDown);
	btnManager.add(goUp);
	btnManager.add(goLeft);
	btnManager.add(goRight);

	btnManager.loadAllText(mainRenderer);
	
	manager.initAll2d(mainRenderer);

	bool isRunning = true;

	while (isRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			isRunning = program.processEvent(event);
			btnManager.handleAllEvent(event);
			program.programControls();
		}

		SDL_SetRenderDrawColor(mainRenderer, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
		SDL_RenderClear(mainRenderer);

		manager.renderAll2d(mainRenderer);
		btnManager.renderAll(mainRenderer);

		SDL_RenderPresent(mainRenderer);
	}
	program.onQuit(mainWindow, mainRenderer);

	return 0;
}