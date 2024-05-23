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
    SDL_Texture* pressEnterTexture;
    SDL_Texture* squareTexture;

    int characterX;
    int characterY;
    int ground1X;
    int ground2X;
    int ground3X;
    int groundY;
    int squareX, squareY;
    const int squareW = 15;
    const int max_squareH = 250;
    const float growthRate = 20.0f;
    float squareH;

    Uint32 startTime = SDL_GetTicks();

    int d12;
    int d23;
    int moveDistance;

    bool moveGrounds;
    bool spacePressed;
    bool enterPressed;


    void initSDL();
    void loadTextures();
    void calculateGroundPositions();
    void handleEvents();
    void squareGrowth();
    void update();
    void render();
    void clean();
};

#endif
