// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/button.h"
#include "../include/textBox.h"
#include "../include/buttonManager.h"
#include "../include/image.h"
#include "../include/point.h"
#include "../include/angles.h"
#include "../include/vector.h"

void bresenham(Point2 p0, Point2 p1, std::vector<std::vector<image>> &list) {

	int height = list.size();
	if (height == 0) return;
	int width = list[0].size();

	auto isInsideBounds = [width, height](int x, int y) -> bool {
		return x >= 0 && x < width && y >= 0 && y < height;
	};

	int sx = (p0.x < p1.x) ? 1 : -1;
	int sy = (p0.y < p1.y) ? 1 : -1;

	int dx = std::abs(p1.x - p0.x);
	int dy = -std::abs(p1.y - p0.y);

	int error = dx + dy;

	while (true) {
		if (isInsideBounds(p0.x, p0.y)) {
			list[p0.y][p0.x].filePath = "./res/imgs/lamp_on.png";
			// std::cout << "Specified location: (" << p0.x << " " << p0.y << ")\n";
		}
		// std::cout << "Current location: (" << p0.x << " " << p0.y << ")\n";
		if (p0.x == p1.x && p0.y == p1.y) break;
		int e2 = 2 * error;
		if (e2 >= dy) {
			error += dy;
			p0.x += sx;
		}
		if (e2 <= dx) {
			error += dx;
			p0.y += sy;
		}
	}
}

// initialization
SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

// window's color, change please
SDL_Color windowColor = {255, 255, 255 ,255};

imageManager manager;
buttonManager btnManager;

Point3 camPos(0, 0, 0); // C_xyz
Point3 displayPos(0, 0, 10); // E_xyz

EulerAngles boxAngles(0, 0, 0); // theta_xyz

Point3 p1(10, 7, 7);
Point3 p2(5, 7, 7);
Point3 p3(5, 7, 2);
Point3 p4(10, 7, 2);

Point3 p5(10, 2, 7);
Point3 p6(5, 2, 7);
Point3 p7(5, 2, 2);
Point3 p8(10, 2, 2);

std::vector<Point3> vertices = {p1, p2, p3, p4, p5, p6, p7, p8}; // A points (A_xyz)


std::vector<Vector3> d_vs(8, Vector3(0, 0, 0)); // D_xyz

std::vector<Point2> projectionPs(8, Point2(0, 0)); // B_xy


void updateAngles(EulerAngles boxAngles, Point3 camPos, Point3 displayPos) {
	for (int i = 0; i < d_vs.size(); i++) {
		d_vs[i].x = (EulerToMatrixLH(boxAngles) * (vertices[i] - camPos)).x;
		d_vs[i].y = (EulerToMatrixLH(boxAngles) * (vertices[i] - camPos)).y;
		d_vs[i].z = (EulerToMatrixLH(boxAngles) * (vertices[i] - camPos)).z;
	}
	for (int i = 0; i < projectionPs.size(); i++) {
		projectionPs[i].x = (displayPos.z / d_vs[i].z) * d_vs[i].x + displayPos.x;
		projectionPs[i].y = (displayPos.z / d_vs[i].z) * d_vs[i].y  + displayPos.y;
	}
}


int tilesCountX = 120, tilesCountY = 120;

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

	btnManager.add(goDown);
	btnManager.add(goUp);
	btnManager.add(goLeft);
	btnManager.add(goRight);

	btnManager.loadAllText(mainRenderer);
	
	manager.initAll2d(mainRenderer);

	bool isRunning = true;

	updateAngles(boxAngles, camPos, displayPos);
	for (Point2& pt0 : projectionPs) {
		for (Point2& pt1 : projectionPs) {
			if (&pt0 == &pt1) continue;
			bresenham(pt0, pt1, manager.images2d);
		}
	}

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