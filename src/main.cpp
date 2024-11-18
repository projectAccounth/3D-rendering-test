// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/button.h"
#include "../include/textBox.h"
#include "../include/buttonManager.h"
#include "../include/image.h"
#include "../include/point.h"
#include "../include/angles.h"
#include "../include/vector.h"
#include "../include/algorithms.h"

int tilesCountX = 80, tilesCountY = 80;

// initialization
SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

// window's color, change please
SDL_Color windowColor = {255, 255, 255 ,255};

imageManager manager;
buttonManager btnManager;

Point3 camPos(0, 0, -30); // C_xyz
Point3 displayPos(50, 50, -20); // E_xyz

EulerAngles camRotation(0, 0, 0); // theta_xyz
EulerAngles boxRotation(0, 0, 0);

// top
Point3 p1(10, 10, 10);
Point3 p2(10, 10, -10);
Point3 p3(-10, 10, -10);
Point3 p4(-10, 10, 10);

// bottom
Point3 p5(10, -10, 10);
Point3 p6(10, -10, -10);
Point3 p7(-10, -10, -10);
Point3 p8(-10, -10, 10);

float camFOV = 2 * std::atan(std::pow(tilesCountX, 2) / 4 * displayPos.z);

std::vector<Point3> vertices = {p1, p2, p3, p4, p5, p6, p7, p8}; // A points (A_xyz)

std::vector<Vector3> d_vs(8, Vector3(0, 0, 0)); // D_xyz

std::vector<Point2> projectionPs(8, Point2(0, 0)); // B_xy




int main(int argc, char* argv[]) {

	manager.fill2d(0, 0, tilesCountX, tilesCountY, WINDOW_WIDTH / tilesCountX, (WINDOW_HEIGHT - WINDOW_HEIGHT_OFFSET) / tilesCountY, "./res/imgs/lamp.png");

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
		boxRotation.pitch -= 0.1f;
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, camPos, displayPos, vertices, projectionPs);
		// updateAngles(camRotation, camPos, displayPos);
		updateImage(vertices, projectionPs, manager);
	}));

	goUp.setAction(std::bind([&]() {
		boxRotation.pitch += 0.1f;
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, camPos, displayPos, vertices, projectionPs);
		// updateAngles(camRotation, camPos, displayPos);
		updateImage(vertices, projectionPs, manager);
	}));

	goLeft.setAction(std::bind([&]() {
		boxRotation.yaw -= 0.1f;
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, camPos, displayPos, vertices, projectionPs);
		// updateAngles(camRotation, camPos, displayPos);
		updateImage(vertices, projectionPs, manager);
	}));

	goRight.setAction(std::bind([&]() {
		boxRotation.yaw += 0.1f;
		transformObject(boxRotation, calculateCentroid(vertices), camRotation, camPos, displayPos, vertices, projectionPs);
		// updateAngles(camRotation, camPos, displayPos);
		updateImage(vertices, projectionPs, manager);
	}));

	btnManager.add(goDown);
	btnManager.add(goUp);
	btnManager.add(goLeft);
	btnManager.add(goRight);

	btnManager.loadAllText(mainRenderer);
	
	manager.initAll2d(mainRenderer);

	bool isRunning = true;

	updateAngles(camRotation, camPos, displayPos, d_vs, vertices, projectionPs);
	transformObject(boxRotation, calculateCentroid(vertices), camRotation, camPos, displayPos, vertices, projectionPs);
	updateImage(vertices, projectionPs, manager);

	while (isRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			isRunning = program.processEvent(event);
			btnManager.handleAllEvent(event);
			program.programControls(camPos);
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