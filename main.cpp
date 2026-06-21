#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    // 1. Initialize SDL Video Subsystem
    if (SDL_Init(SDL_INIT_VIDEO |  SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Open the audio device with common settings
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError();
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF Init Error: " << TTF_GetError() << std::endl;
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    // 2. Create Window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Template Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        SDL_CloseAudio();
        return 1;
    }

    // 3. Create Hardware-Accelerated Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        SDL_CloseAudio();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("Fonts/PressStart2P-Regular.ttf", 24);

    if (!font) {
        std::cerr << "Font Load Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // 4. Main Game Loop Variables
    bool isRunning = true;
    SDL_Event event;

    // 5. Main Game Loop
    while (isRunning) {
        // Handle events (input, closing window, etc.)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
            }
        }

        // Clear the screen (Background Color: Dark Grey)
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // --- DRAW YOUR GRAPHICS HERE ---
        // Example: Draw a centered red rectangle
        SDL_Rect rect = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 100, 100 };
        SDL_SetRenderDrawColor(renderer, 230, 50, 50, 255);
        SDL_RenderFillRect(renderer, &rect);
        // -------------------------------

        // Update the screen display
        SDL_RenderPresent(renderer);
    }

    // 6. Cleanup and Shutdown Resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    return 0;
}

