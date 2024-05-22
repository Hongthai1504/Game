#include "game.h"
#include <bits/stdc++.h>

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(&window, &renderer)) {
        std::cerr << "Failed to initialize SDL.\n";
        return -1;
    }

    SDL_Texture* backgroundTexture = loadTexture("images/background.png", renderer);
    SDL_Texture* characterTexture = loadTexture("images/character.png", renderer);
    SDL_Texture* groundTexture = loadTexture("images/ground.png", renderer);

    if (!backgroundTexture || !characterTexture || !groundTexture) {
        std::cerr << "Failed to load textures.\n";
        close(window, renderer);
        return -1;
    }

    std::srand(std::time(0));
    int ground1_x = GROUND1_X;
    int ground2_x = ground1_x + MIN_DISTANCE + (std::rand() % (MAX_DISTANCE - MIN_DISTANCE + 1));
    int ground3_x = ground2_x + MIN_DISTANCE + (std::rand() % (MAX_DISTANCE - MIN_DISTANCE + 1));

    bool quit = false;
    bool spacePressed = false;

    while (!quit) {
        handleEvents(quit, spacePressed);
        updateGroundPositions(ground1_x, ground2_x, ground3_x, spacePressed, MAX_DISTANCE - MIN_DISTANCE);

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        SDL_Rect ground1Rect = { ground1_x, GROUND_Y, GROUND_WIDTH, GROUND_HEIGHT };
        SDL_Rect ground2Rect = { ground2_x, GROUND_Y, GROUND_WIDTH, GROUND_HEIGHT };
        SDL_Rect ground3Rect = { ground3_x, GROUND_Y, GROUND_WIDTH, GROUND_HEIGHT };

        SDL_RenderCopy(renderer, groundTexture, nullptr, &ground1Rect);
        SDL_RenderCopy(renderer, groundTexture, nullptr, &ground2Rect);
        SDL_RenderCopy(renderer, groundTexture, nullptr, &ground3Rect);

        SDL_Rect characterRect = { GROUND1_X + (GROUND_WIDTH - CHARACTER_WIDTH) / 2, GROUND_Y - CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT };
        SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);

        SDL_RenderPresent(renderer);
    }

    close(window, renderer);
    return 0;
}
