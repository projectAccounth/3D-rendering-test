// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/button.h"
#include "../include/textBox.h"
#include "../include/buttonManager.h"
#include "../include/image.h"
#include "../include/point.h"
#include "../include/angles.h"
#include "../include/vector.h"

void bresenham(Point2 p0, Point2 p1, std::vector<std::vector<image>>& list) {

	int height = list.size();
	if (height == 0) return;

	int width = list[0].size();

	auto isInsideBounds = [width, height](int x, int y) -> bool {
		return x >= 0 && x < width && y >= 0 && y < height;
	};

	std::cout << "p1: " << p1.x << ' ' << p1.y << '\n';

	// Round the floating-point coordinates to integers for Bresenham
	int x0 = static_cast<int>(std::round(p0.x));
	int y0 = static_cast<int>(std::round(p0.y));
	int x1 = static_cast<int>(std::round(p1.x));
	int y1 = static_cast<int>(std::round(p1.y));

	// std::cout << x0 << ' ' << x1;

	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;

	if (x0 == x1) sx = 0;
	if (y0 == y1) sy = 0;

	// Calculate the absolute differences in coordinates
	int dx = std::abs(x1 - x0);
	int dy = -std::abs(y1 - y0);

	// Initialize the error term for integer precision
	int error = dx + dy;

	p0 = { static_cast<float>(x0), static_cast<float>(y0) };

	while (true) {
		// Use the integer rounded coordinates to set the image at the pixel location
		if (isInsideBounds(x0, y0)) {
			list[y0][x0].filePath = "./res/imgs/lamp_on.png";
			std::cout << "Specified location: (" << x0 << " " << y0 << ")\n";
		}

		std::cout << "Current location: (" << x0 << " " << y0 << ")\n";
		std::cout << "Current location (1): (" << x1 << " " << y1 << ")\n";

		p0 = { static_cast<float>(x0), static_cast<float>(y0) }; // Update the last rounded point

		if (x0 == x1 && y0 == y1) return;

		int e2 = 2 * error;

		if (e2 >= dy) {
			error += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			error += dx;
			y0 += sy;
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

Point3 camPos(10, 20, 0); // C_xyz
Point3 displayPos(10, 20, 10); // E_xyz

EulerAngles boxAngles(0, 0, 0); // theta_xyz

// top
Point3 p1(10, 30, 10);
Point3 p2(20, 30, 10);
Point3 p3(20, 30, 20);
Point3 p4(10, 30, 20);

// bottom
Point3 p5(10, 20, 10);
Point3 p6(20, 20, 10);
Point3 p7(20, 20, 20);
Point3 p8(10, 20, 20);

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
		if (d_vs[i].z != 0) {
			projectionPs[i].x = (displayPos.z / d_vs[i].z) * d_vs[i].x + displayPos.x;
			projectionPs[i].y = (displayPos.z / d_vs[i].z) * d_vs[i].y + displayPos.y;
		}
		else {
			// Handle edge case (e.g., avoid division by zero or apply an alternative method)
			// For example, you could skip rendering, or set to a default value
			projectionPs[i].x = displayPos.x;
			projectionPs[i].y = displayPos.y;
		}
	}
}

void updateImage() {
	for (int i = 0; i < projectionPs.size(); i++) {
		for (int j = 0; j < projectionPs.size(); j++) {
			if (&projectionPs[i] == &projectionPs[j]) continue;

			bool isAdjacent3D = false;
			const Point3& pt0_3D = vertices[i];  // 3D position of projectionPs[i]
			const Point3& pt1_3D = vertices[j];  // 3D position of projectionPs[j]

			if (pt0_3D.y == pt1_3D.y && pt0_3D.z == pt1_3D.z) {
				isAdjacent3D = true;  // Adjacent along the x-axis
			}
			else if (pt0_3D.x == pt1_3D.x && pt0_3D.z == pt1_3D.z) {
				isAdjacent3D = true;  // Adjacent along the y-axis
			}
			else if (pt0_3D.x == pt1_3D.x && pt0_3D.y == pt1_3D.y) {
				isAdjacent3D = true;  // Adjacent along the z-axis
			}

			if (!isAdjacent3D) continue;

			bresenham(projectionPs[i], projectionPs[j], manager.images2d);
		}
	}
}


int tilesCountX = 48, tilesCountY = 48;

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
	updateImage();

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