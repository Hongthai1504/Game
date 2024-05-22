#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* characterTexture;
    SDL_Texture* groundTexture;

    int characterX;
    int characterY;
    int ground1X;
    int ground2X;
    int ground3X;
    int groundY;

    int d12; // Khoảng cách ban đầu giữa ô đất thứ 1 và ô đất thứ 2
    int d23; // Khoảng cách ban đầu giữa ô đất thứ 2 và ô đất thứ 3
    int moveDistance; // Khoảng cách đã di chuyển

    bool moveGrounds; // Flag to indicate if grounds should move
    bool spacePressed; // Flag to indicate if space was pressed

    void initSDL();
    void loadTextures();
    void calculateGroundPositions();
    void handleEvents();
    void update();
    void render();
    void clean();
};

#endif
