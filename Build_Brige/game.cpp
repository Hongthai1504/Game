#include "game.h"
#include <bits/stdc++.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : moveGrounds(false), moveDistance(0), spacePressed(false) {
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
    if (!backgroundTexture || !characterTexture || !groundTexture) {
        std::cerr << "Failed to load textures." << std::endl;
        clean();
        exit(1);
    }
}

void Game::calculateGroundPositions() {
    characterX = 220;
    characterY = 225;
    srand(time(nullptr));

    ground1X = 223;

    // Tạo một khoảng cách ngẫu nhiên giữa ô đất 1 và 2, và giữ cho nó ít nhất là 250 đơn vị
    d12 = 250 + rand() % 101; // Khoảng cách tối thiểu là 250, tối đa là 350
    ground2X = ground1X + d12;

    // Tạo một khoảng cách ngẫu nhiên giữa ô đất 2 và 3, và giữ cho nó ít nhất là 250 đơn vị
    d23 = 250 + rand() % 101; // Khoảng cách tối thiểu là 250, tối đa là 350
    ground3X = ground2X + d23;

    groundY = 375;
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
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            spacePressed = true;
        }
    }
}

void Game::update() {
    if (moveGrounds) {
        // Move all grounds to the left until they have moved by d12
        if (moveDistance < d12) {
            ground1X -= 2;
            ground2X -= 2;
            ground3X -= 2;
            moveDistance += 2;
        } else {
            moveGrounds = false; // Stop moving once the distance is covered
            spacePressed = false; // Reset space pressed

            // Swap positions and recalculate distances
            int newGround3X = ground3X + 250 + rand() % 101; // Vị trí ngẫu nhiên sau ô đất thứ 3 với khoảng cách từ 250 đến 350
            ground1X = ground2X;
            ground2X = ground3X;
            ground3X = newGround3X;

            // Recalculate d12 and d23
            d12 = ground2X - ground1X;
            d23 = ground3X - ground2X;
        }
    } else if (spacePressed) {
        moveGrounds = true; // Start moving the grounds
        moveDistance = 0;   // Reset move distance
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
    SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(groundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
