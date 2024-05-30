#include "game.h"
#include <bits/stdc++.h>
#include <SDL_image.h>

Game::Game() : moveGrounds(false), moveDistance(0), spacePressed(false), enterPressed(false), growing(false){
    initSDL();
    loadTextures();
    calculateGroundPositions();
    startTime = SDL_GetTicks();
    lastTick = startTime;
}

Game::~Game() {
    clean();
}

void Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1048, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

void Game::loadTextures() {
    backgroundTexture = IMG_LoadTexture(renderer, "images/background.png");
    characterTexture = IMG_LoadTexture(renderer, "images/character.png");
    groundTexture = IMG_LoadTexture(renderer, "images/ground.png");
    pressEnterTexture = IMG_LoadTexture(renderer, "images/pressEnter.png");
    squareTexture = IMG_LoadTexture(renderer, "images/square.png");

    if (!backgroundTexture || !characterTexture || !groundTexture) {
        std::cerr << "Failed to load textures." << std::endl;
        clean();
        exit(1);
    }
}

void Game::calculateGroundPositions() {
    characterX = 210;
    characterY = 220;
    srand(time(nullptr));

    ground1X = 220;

    d12 = 250 + (rand() % (100/2)*2);
    ground2X = ground1X + d12;

    d23 = 250 + (rand() % (100/2)*2);
    ground3X = ground2X + d23;

    groundY = 360;
}

void Game::run() {
    bool quit = false;
    while (!quit) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            clean();
            exit(0);
        }
        else if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case SDLK_RETURN:
                    enterPressed = true;
                    enterTime = SDL_GetTicks();
                    break;
                case SDLK_SPACE:
                    if (enterPressed){
                        spacePressed = true;
                    }
                    break;
            }
        }
    }
}

void Game::update() {

    squareX = 320;
    squareY = 360;
    squareH = 0.0f;

    Uint32 currentTime = SDL_GetTicks();
    if (!growing && enterPressed && ((currentTime - startTime) / 1000.0f >= startDelay)){
        growing = true;
        startTime = currentTime;
    }
    if (growing && !spacePressed){
        float elapsedTime = currentTime - startTime;
        squareH = (elapsedTime / 1000.0) * growthRate;
        if (squareH >= max_squareH){
        squareH = max_squareH;
        growing = false;
        }
    }

    if (moveGrounds) {
        if (moveDistance < d12) {
            ground1X -= 6;
            ground2X -= 6;
            ground3X -= 6;
            moveDistance += 6;
        } else {
            moveGrounds = false;
            spacePressed = false;

            int newGround3X = ground3X + 250 + (rand() % (100/2)*2);
            ground1X = 220;
            ground2X = ground3X + 2;
            ground3X = newGround3X;

            d12 = ground2X - ground1X;
            d23 = ground3X - ground2X;
        }
    } else if (spacePressed) {
        moveGrounds = true;
        moveDistance = 0;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    SDL_Rect groundRect1 = {ground1X, groundY, 100, 120};
    SDL_Rect groundRect2 = {ground2X, groundY, 100, 120};
    SDL_Rect groundRect3 = {ground3X, groundY, 100, 120};
    SDL_RenderCopy(renderer, groundTexture, nullptr, &groundRect1);
    SDL_RenderCopy(renderer, groundTexture, nullptr, &groundRect2);
    SDL_RenderCopy(renderer, groundTexture, nullptr, &groundRect3);
    SDL_Rect characterRect = {characterX, characterY, 150, 180};
    SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);

    SDL_Rect squareRect = {squareX, squareY - (int)squareH, squareW, (int)squareH};
    if (enterPressed){
        SDL_RenderCopy(renderer, squareTexture, nullptr, &squareRect);
    }
    if (!enterPressed){
            SDL_RenderCopy(renderer, pressEnterTexture, nullptr, nullptr);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(groundTexture);
    SDL_DestroyTexture(pressEnterTexture);
    SDL_DestroyTexture(squareTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
