#pragma once

// Include guard
#ifndef MAIN_HDR_H
#define MAIN_HDR_H

#define WINDOW_HEIGHT_OFFSET 40
// These are the window width and height, can be c8hanged
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 520

const int tilesCountX = 80, tilesCountY = 80;

const float nearPlane = 0.1f;

// Includes for the program

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <functional>
#include <variant>
#include <type_traits>

enum textAlign {
    LEFT,
    CENTER,
    RIGHT
};

#endif /* MAIN_HDR_H */
