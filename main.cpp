#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>

// Screen dimensions
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 900;

float shotCooldown = 0.0f;

int CREDITS = 00;
int SCORE = 0000;
int HISCORE = 0000;
const int SPEED = 10;
const int playerW = 150;
const int playerH = 50;
int playerX = SCREEN_WIDTH / 2 - playerW / 2;
int playerY = 770;

struct Bullet {
  SDL_Rect bulletrect;
  int x, y, dy;
  bool active = true;

  void update() {
    y += dy;
  }
};

std::vector<Bullet> bullets;

void updateBullets(std::vector<Bullet>& bullets) {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->update();
        

        // Remove bullet if it goes off the top of the screen
        if (it->y < 0) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}


void renderBullets(SDL_Renderer* renderer, std::vector<Bullet>& bullets) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color
    
    for (auto& bullet : bullets) {
        bullet.bulletrect = { static_cast<int>(bullet.x), static_cast<int>(bullet.y), 8, 16 };
        SDL_RenderFillRect(renderer, &bullet.bulletrect);
    }
}


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


    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "Sprites/player.png");
    if (!playerTexture) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
    }

    SDL_SetTextureBlendMode(playerTexture, SDL_BLENDMODE_BLEND);


    struct GameObject {
      SDL_Rect destrect;
      bool active = true;

      GameObject() {
        destrect.x = 150;
        destrect.y = 125;
        destrect.w = 100;
        destrect.h = 100;
      }
    };





    std::vector<GameObject> instances;

    GameObject obj;
    for(int i = 0; i < 11; i++) {
      instances.push_back(obj);
      obj.destrect.x += 100;
    }
  
    obj.destrect.y += 100;
    obj.destrect.x = 125;

    for(int i = 0; i < 11; i++) {
      instances.push_back(obj);
      obj.destrect.x += 100;
    }

    obj.destrect.y += 100;
    obj.destrect.x = 150;

    for(int i = 0; i < 11; i++) {
      instances.push_back(obj);
      obj.destrect.x += 100;
    }

    obj.destrect.y += 100;
    obj.destrect.x = 125;

    for(int i = 0; i < 11; i++) {
      instances.push_back(obj);
      obj.destrect.x += 100;
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface*  creditsSurface = TTF_RenderText_Blended(font, "CREDITS", textColor);
    SDL_Texture* creditsTexture = SDL_CreateTextureFromSurface(renderer, creditsSurface);

    SDL_Surface*  CREDITSSurface = TTF_RenderText_Blended(font, std::to_string(CREDITS).c_str(), textColor);
    SDL_Texture* CREDITSTexture = SDL_CreateTextureFromSurface(renderer, CREDITSSurface);

    SDL_Surface*  scoreSurface = TTF_RenderText_Blended(font, "SCORE", textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    SDL_Surface*  SCORESurface = TTF_RenderText_Blended(font, std::to_string(SCORE).c_str(), textColor);
    SDL_Texture* SCORETexture = SDL_CreateTextureFromSurface(renderer, SCORESurface);

    SDL_Surface*  hiscoreSurface = TTF_RenderText_Blended(font, "HIGH SCORE", textColor);
    SDL_Texture* hiscoreTexture = SDL_CreateTextureFromSurface(renderer, hiscoreSurface);

    SDL_Surface*  HISCORESurface = TTF_RenderText_Blended(font, std::to_string(HISCORE).c_str(), textColor);
    SDL_Texture* HISCORETexture = SDL_CreateTextureFromSurface(renderer, HISCORESurface);

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

    SDL_FreeSurface(scoreSurface);



    SDL_Rect SCORErect;
    SCORErect.x = 200;
    SCORErect.y = 50;
    SCORErect.w = SCORESurface->w;
    SCORErect.h = SCORESurface->h;

    SDL_FreeSurface(SCORESurface);



    SDL_Rect hiscorerect;
    hiscorerect.x = 900;
    hiscorerect.y = 50;
    hiscorerect.w = hiscoreSurface->w;
    hiscorerect.h = hiscoreSurface->h;

    SDL_FreeSurface(hiscoreSurface);

    SDL_Rect HISCORErect;
    HISCORErect.x = 1200;
    HISCORErect.y = 50;
    HISCORErect.w = HISCORESurface->w;
    HISCORErect.h = HISCORESurface->h;

    SDL_FreeSurface(HISCORESurface);



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

        playerX = std::clamp(playerX, 0, SCREEN_WIDTH - playerW); 

        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
          playerX -= SPEED;
        }

        if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
          playerX += SPEED;
        }


        if (state[SDL_SCANCODE_SPACE] && shotCooldown <= 0.0f) {
          Bullet newBullet;
          newBullet.x = playerX + playerW / 2;
          newBullet.y = playerY;
          newBullet.dy = -15;
          newBullet.active = true;

          bullets.push_back(newBullet);
          shotCooldown = 0.25f;
        }

        if (shotCooldown > 0) {
          shotCooldown -= 0.016f;  // Minus 16ms per frame assuming 60FPS this equals a quarter second.
        }


        // Color Screen background
        SDL_SetRenderDrawColor(renderer, 30, 50, 70, 255);
        SDL_RenderClear(renderer);

        // --- DRAW YOUR GRAPHICS HERE ---
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 825, 1400, 825);

        SDL_Rect PLAYER {playerX, playerY, playerW, playerH};

        SDL_RenderCopy(renderer, playerTexture, NULL, &PLAYER);



        // Render Enemies
        for (const auto& instance : instances) {
          SDL_RenderCopy(renderer, imageTexture, NULL, &instance.destrect);
        
        }


        // STEP 1: Detect collisions and flag entities as inactive
        for (auto& bullet : bullets) {
          // If the bullet is already inactive, skip it
          if (!bullet.active) continue; 

          for (auto& instance : instances) {
            // If the enemy instance is already inactive, skip it
            if (!instance.active) continue;

            // Check the intersection
            if (SDL_HasIntersection(&instance.destrect, &bullet.bulletrect)) {
              bullet.active = false;   // Mark bullet as inactive
              instance.active = false; // Mark enemy instance as inactive

              break; // Stop checking this bullet; it already hit something
            }
          }
        }

        // Erase elements where active is false
        std::erase_if(bullets, [](const Bullet& b) { return !b.active; });
        std::erase_if(instances, [](const GameObject& i) { return !i.active; });

        renderBullets(renderer, bullets);
        updateBullets(bullets);


        //Render all Fonts 
        SDL_RenderCopy(renderer, creditsTexture, NULL, &creditsrect);
        SDL_RenderCopy(renderer, CREDITSTexture, NULL, &CREDITSrect);
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scorerect);
        SDL_RenderCopy(renderer, SCORETexture, NULL, &SCORErect);
        SDL_RenderCopy(renderer, hiscoreTexture, NULL, &hiscorerect);
        SDL_RenderCopy(renderer, HISCORETexture, NULL, &HISCORErect);
        SDL_RenderCopy(renderer, playerTexture, NULL, &PLAYER);

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

