#ifndef GAME_H
#define GAME_H

#include <SDL.h>

bool init(SDL_Window** window, SDL_Renderer** renderer);
SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
void close(SDL_Window* window, SDL_Renderer* renderer);
void handleEvents(bool &quit, bool &spacePressed);
void updateGroundPositions(int &ground1_x, int &ground2_x, int &ground3_x, bool spacePressed, int square);

const int WINDOW_WIDTH = 1048;
const int WINDOW_HEIGHT = 600;

const int CHARACTER_WIDTH = 148;
const int CHARACTER_HEIGHT = 180;

const int GROUND_WIDTH = 100;
const int GROUND_HEIGHT = 120;

const int GROUND1_X = 223;
const int GROUND_Y = 375;

const int MIN_DISTANCE = 250;
const int MAX_DISTANCE = 350;
const int MOVE_SPEED = 2; // Tốc độ di chuyển của ô đất

#endif // GAME_H
