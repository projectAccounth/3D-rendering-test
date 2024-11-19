// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/program.h"
#include "../include/button.h"
#include "../include/textBox.h"
#include "../include/buttonManager.h"
#include "../include/image.h"
#include "../include/point.h"
#include "../include/angles.h"
#include "../include/vector.h"
#include "../include/algorithms.h"
#include "../include/boxes.h"

// initialization
SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

// window's color, change please
SDL_Color windowColor = {255, 255, 255 ,255};

imageManager manager;
buttonManager btnManager;
BoxManager boxManager;

Point3 camPos(tilesCountX / 2, tilesCountY / 2, -30); // C_xyz
Point3 displayPos(tilesCountX / 2, tilesCountY / 2, -20); // E_xyz

EulerAngles camRotation(0, 0, 0); // theta_xyz
EulerAngles boxRotation(0, 0, 0);



double camFOV = 60 * M_PI / 180;// 2 * std::atan(std::pow(tilesCountX, 2) / 4 * std::abs(displayPos.z));

std::vector<Vector3> d_vs(16, Vector3(0, 0, 0)); // D_xyz

std::vector<Point2> projectionPs(16, Point2(0, 0)); // B_xy




int main(int argc, char* argv[]) {
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 1; j++) {
			boxManager.add(Box(Point3(10 + 50 * i, 10, 10 + 50 * j), 50));
		}
	}

	std::vector<Point3> totalVertices = boxManager.boxes[0].vertices;

	for (int i = 1; i < boxManager.boxes.size(); i++) {
		totalVertices.insert(totalVertices.end(), boxManager.boxes[i].vertices.begin(), boxManager.boxes[i].vertices.end());
	}

	manager.fill2d(0, 0, tilesCountX, tilesCountY, WINDOW_WIDTH / tilesCountX, (WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET) / tilesCountY, "./res/imgs/orsh_1.png");

	Program program;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	TTF_Font* mssnsf = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

	if (mssnsf == nullptr) {
		std::cout << "A problem occurred when creating font: " << TTF_GetError() << '\n';
	}

	mainWindow = program.createWindow(WINDOW_HEIGHT, WINDOW_WIDTH, 0, "Program");

	mainRenderer = program.createRenderer(mainWindow);

	int btnHeight = WINDOW_HEIGHT_OFFSET;

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

	goDown.setAction(std::bind([&]() {
		resetAllImages(manager.images2d);
		camRotation.pitch += M_PI / 16;
		updateAll(boxRotation, calculateCentroid(totalVertices),
			camRotation, camPos,
			displayPos, totalVertices,
			projectionPs, d_vs, manager);
	}));

	goUp.setAction(std::bind([&]() {
		resetAllImages(manager.images2d);
		camRotation.pitch -= M_PI / 16;
		updateAll(boxRotation, calculateCentroid(totalVertices),
			camRotation, camPos,
			displayPos, totalVertices,
			projectionPs, d_vs, manager);
	}));

	goLeft.setAction(std::bind([&]() {
		resetAllImages(manager.images2d);
		camRotation.yaw += M_PI / 16;
		updateAll(boxRotation, calculateCentroid(totalVertices),
			camRotation, camPos,
			displayPos, totalVertices,
			projectionPs, d_vs, manager);
	}));

	goRight.setAction(std::bind([&]() {
		resetAllImages(manager.images2d);
		camRotation.yaw -= M_PI / 16;
		updateAll(boxRotation, calculateCentroid(totalVertices),
			camRotation, camPos,
			displayPos, totalVertices,
			projectionPs, d_vs, manager);
	}));

	btnManager.add(goDown);
	btnManager.add(goUp);
	btnManager.add(goLeft);
	btnManager.add(goRight);

	btnManager.loadAllText(mainRenderer);
	
	manager.initAll2d(mainRenderer);

	bool isRunning = true;

	resetAllImages(manager.images2d);
	updateAll(boxRotation, calculateCentroid(totalVertices),
			camRotation, camPos,
			displayPos, totalVertices,
			projectionPs, d_vs, manager);

	while (isRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			isRunning = program.processEvent(event);
			btnManager.handleAllEvent(event);
			program.programControls(camRotation, camPos, displayPos, d_vs, totalVertices, projectionPs, boxRotation, manager);
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