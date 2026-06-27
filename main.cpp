#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <iostream>

// Screen dimensions
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 900;


int CREDITS = 00;
int SCORE = 0000;

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

    SDL_Texture* imageTexture = IMG_LoadTexture(renderer, "Sprites/invader.png");
    if (!imageTexture) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
    }



    // Rect for First sample image.
    SDL_Rect destrect;
    destrect.x = 150;
    destrect.y = 125;
    destrect.w = 100;
    destrect.h = 100;



    SDL_Color textColor = {255, 255, 255};
    SDL_Surface*  creditsSurface = TTF_RenderText_Blended(font, "CREDITS", textColor);
    SDL_Texture* creditsTexture = SDL_CreateTextureFromSurface(renderer, creditsSurface);

    SDL_Surface*  CREDITSSurface = TTF_RenderText_Blended(font, std::to_string(CREDITS).c_str(), textColor);
    SDL_Texture* CREDITSTexture = SDL_CreateTextureFromSurface(renderer, CREDITSSurface);

    SDL_Surface*  scoreSurface = TTF_RenderText_Blended(font, "SCORE", textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);


    // Rectangles to place fonts need to change hardcoded values for variables.
    SDL_Rect creditsrect;
    creditsrect.x = 1150;
    creditsrect.y = 850;
    creditsrect.w = creditsSurface->w;
    creditsrect.h = creditsSurface->h;

    SDL_FreeSurface(creditsSurface);

    SDL_Rect CREDITSrect;
    CREDITSrect.x = 1350;
    CREDITSrect.y = 850;
    CREDITSrect.w = CREDITSSurface->w;
    CREDITSrect.h = CREDITSSurface->h;

    SDL_FreeSurface(CREDITSSurface);

    SDL_Rect livesrect;




    SDL_Rect scorerect;
    scorerect.x = 50;
    scorerect.y = 50;
    scorerect.w = scoreSurface->w;
    scorerect.h = scoreSurface->h;




    SDL_Rect SCORErect;




    SDL_Rect hiscorerect;





    SDL_Rect HISCORErect;




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

        // Color Screen background
        SDL_SetRenderDrawColor(renderer, 30, 50, 70, 255);
        SDL_RenderClear(renderer);

        // --- DRAW YOUR GRAPHICS HERE ---
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 825, 1400, 825);
        SDL_RenderCopy(renderer, imageTexture, NULL, &destrect);

        SDL_RenderCopy(renderer, creditsTexture, NULL, &creditsrect);
        SDL_RenderCopy(renderer, CREDITSTexture, NULL, &CREDITSrect);
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scorerect);

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

