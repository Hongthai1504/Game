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
    const float max_squareH = 350;
    const float growthRate = 25.0f;
    float squareH;
    const float startDelay = 3.0f;

    Uint32 startTime;
    Uint32 enterTime;
    Uint32 lastTick;

    int d12;
    int d23;
    int moveDistance;

    bool moveGrounds;
    bool spacePressed;
    bool enterPressed;
    bool growing;


    void initSDL();
    void loadTextures();
    void calculateGroundPositions();
    void handleEvents();
    void update();
    void render();
    void clean();
};

#endif
