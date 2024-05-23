#include "game.h"
#include <bits/stdc++.h>
#include <SDL_image.h>

Game::Game() : moveGrounds(false), columnHorizontal(false), moveDistance(0), spacePressed(false), enterPressed(false) {
    initSDL();
    loadTextures();
    calculateGroundPositions();
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
    characterY = 224;
    srand(time(nullptr));

    ground1X = 222;

    d12 = 250 + rand() % 101;
    ground2X = ground1X + d12;

    d23 = 250 + rand() % 101;
    ground3X = ground2X + d23;

    groundY = 375;
}

void Game::run() {
    bool quit = false;
    while (!quit) {
        handleEvents();
        squareGrowth();
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
                    enterPressTime = SDL_GetTicks();
                    break;
                case SDLK_SPACE:
                    if (enterPressed && !spacePressed){
                        spacePressed = true;
                        columnHorizontal = true;
                    }
                    break;
            }
        }
    }
}

void Game::squareGrowth(){
    squareX = 330;
    squareY = 375;
    squareH = 0.0f;
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - startTime) / 1000.0f;
    startTime = currentTime;

    if (enterPressTime && (currentTime - enterPressTime>= 3000) && !spacePressed){
        if (squareH < max_squareH) {
            squareH += growthRate * deltaTime;
            if (squareH > max_squareH) {
                squareH = max_squareH;
            }
        }
    }
}
void Game::update() {
    if (moveGrounds) {
        if (moveDistance < d12) {
            ground1X -= 4;
            ground2X -= 4;
            ground3X -= 4;
            moveDistance += 4;
        } else {
            moveGrounds = false;
            spacePressed = false;

            int newGround3X = ground3X + 250 + rand() % 101;
            ground1X = ground2X + 2;
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
    SDL_Rect characterRect = {characterX, characterY, 148, 180};
    if (columnHorizontal) {
        SDL_Rect dstRect = { squareX, squareY - squareW / 2, static_cast<int>(squareH), squareW };
        SDL_RenderCopy(renderer, squareTexture, NULL, &dstRect);
    } else {
        SDL_Rect dstRect = { squareX, squareY - static_cast<int>(squareH), squareW, static_cast<int>(squareH) };
        SDL_RenderCopy(renderer, squareTexture, NULL, &dstRect);
    }
    SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);
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
