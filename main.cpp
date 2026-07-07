#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

// Screen dimensions
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 900;

float shotCooldown = 0.0f;

int lives = 3;
int level = 1;
bool isAlive = true;
int CREDITS = 00;
int SCORE = 0000;
int HISCORE = 0000;
const int SPEED = 10;
const int playerW = 150;
const int playerH = 50;
int playerX = SCREEN_WIDTH / 2 - playerW / 2;
int playerY = 770;
float enemyShotCooldown = 3.0f;
Uint32 lastTime = SDL_GetTicks();
float deltaTime = 0.0f;


struct Bullet {
  SDL_Rect bulletrect;
  int x, y, dy;
  bool active = true;

  void update() {
    y += dy;
  }
};

struct Barrier {
  int x1, y1, x2, y2;
  bool active = true;

  Barrier() {
    x1 = 100;
    y1 = 720;
    x2 = 100;
    y2 = 750;
  }

  void breatTop() {
    y1 -= 15;
  }

  void breakBot() {
    y2 += 15;
  }
};

std::vector<Barrier> barriers;
std::vector<Bullet> bullets;
std::vector<Bullet> enemyBullets;



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

void updateEnemyBullets(std::vector<Bullet>& enemyBullets) {
    for (auto it = enemyBullets.begin(); it != enemyBullets.end(); ) {
        it->update();
        

        // Remove bullet if it goes off the top of the screen
        if (it->y < 0) {
            it = enemyBullets.erase(it);
        } else {
            ++it;
        }
    }
}




void renderBarriers(SDL_Renderer* renderer, std::vector<Barrier> barriers) {
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  for (auto& barrier : barriers) {
    if (barrier.active) {
      SDL_RenderDrawLine(renderer, barrier.x1, barrier.y1, barrier.x2, barrier.y2);
    }
  }
}
void renderBullets(SDL_Renderer* renderer, std::vector<Bullet>& bullets) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color
    
    for (auto& bullet : bullets) {
        bullet.bulletrect = { static_cast<int>(bullet.x), static_cast<int>(bullet.y), 1, 24 };
        SDL_RenderFillRect(renderer, &bullet.bulletrect);
    }
}

void renderEnemyBullets(SDL_Renderer* renderer, std::vector<Bullet>& enemyBullets) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color
    
    for (auto& enemyBullet : enemyBullets) {
        enemyBullet.bulletrect = { static_cast<int>(enemyBullet.x), static_cast<int>(enemyBullet.y), 1, 24 };
        SDL_RenderFillRect(renderer, &enemyBullet.bulletrect);
    }
}

struct GameObject {
  SDL_Rect destrect;
  bool active = true;
  float dx = 2.0f;

  GameObject() {
    destrect.x = 150;
    destrect.y = 125;
    destrect.w = 100;
    destrect.h = 100;
  }

  void update() {
    destrect.x += dx;


  }

};





std::vector<GameObject> instances;


void placeEnemies(std::vector<GameObject>& instances) {

  GameObject obj;

  if (!instances.empty()) return;

  for (int i = 0; i < 11; i++) {
    instances.push_back(obj);
    obj.destrect.x += 100;
  }

  obj.destrect.y += 100;
  obj.destrect.x = 125;

  for (int i = 0; i < 11; i++) {
    instances.push_back(obj);
    obj.destrect.x += 100;
  }

  obj.destrect.y += 100;
  obj.destrect.x = 150;

  for (int i = 0; i < 11; i++) {
    instances.push_back(obj);
    obj.destrect.x += 100;
  }

  obj.destrect.y += 100;
  obj.destrect.x = 125;

  for (int i = 0; i < 11; i++) {
    instances.push_back(obj);
    obj.destrect.x += 100;
  }


}

void moveEnemies(SDL_Renderer* renderer, SDL_Texture* imageTexture) {

  // Render Enemies
  for (auto& instance : instances) {
    SDL_RenderCopy(renderer, imageTexture, NULL, &instance.destrect);
    instance.update();

    if (instance.destrect.x >= SCREEN_WIDTH - 95) {
      for (auto& obj: instances) {
        obj.dx = -obj.dx;
        obj.destrect.y += 5;
      }
    }
    if (instance.destrect.x <= -10) {
      for (auto& enm: instances) {
        enm.dx = -enm.dx;
        enm.destrect.y += 5;
      }
    }
  }
}


void shootEnemyBullets(std::vector<Bullet>& enemyBullets, float deltaTime) {
  // Reduce a global cooldown tracker if you want to limit overall fire rate
  if (enemyShotCooldown > 0.0f) {
    enemyShotCooldown -= deltaTime;
    return; 
  }

  // Only attempt to shoot if there are actually enemies alive
  if (instances.empty()) return;

  // Pick ONE random enemy from your vector to fire
  int randomIndex = rand() % instances.size();
  auto& shooter = instances[randomIndex];

  if (shooter.active) {
    Bullet newEnemyBullet;
    newEnemyBullet.x = shooter.destrect.x + (shooter.destrect.w / 2) - 4;
    newEnemyBullet.y = shooter.destrect.y + shooter.destrect.h;
    newEnemyBullet.dy = 5;
    newEnemyBullet.active = true;
    newEnemyBullet.bulletrect = { newEnemyBullet.x, newEnemyBullet.y, 1, 16 };

    enemyBullets.push_back(newEnemyBullet);

    enemyShotCooldown = 3.0f; 
  }
}

void killPlayer() {
  lives -= 1;
  placeEnemies(instances);
  std::this_thread::sleep_for(2s);
  playerX = SCREEN_WIDTH / 2 - playerW / 2;

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


    placeEnemies(instances);

    

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface*  creditsSurface = TTF_RenderText_Blended(font, "CREDITS", textColor);
    SDL_Texture* creditsTexture = SDL_CreateTextureFromSurface(renderer, creditsSurface);

    SDL_Surface*  CREDITSSurface = TTF_RenderText_Blended(font, std::to_string(CREDITS).c_str(), textColor);
    SDL_Texture* CREDITSTexture = SDL_CreateTextureFromSurface(renderer, CREDITSSurface);

    SDL_Surface*  scoreSurface = TTF_RenderText_Blended(font, "SCORE", textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);


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



    // Initialize barriers vector matching render layout
    for (int b = 0; b < 3; b++) {
      int startX = 100 + (b * 450); // Spaces out 3 blocks
      for (int i = 0; i < 300; i++) {
        Barrier bar;
        bar.x1 = startX + i;
        bar.y1 = 720;
        bar.x2 = startX + i;
        bar.y2 = 750;
        bar.active = true;
        barriers.push_back(bar);
      }
    }

    // 5. Main Game Loop
    while (isRunning) {
      Uint32 currentTime = SDL_GetTicks();
      // Calculate seconds passed since the last frame (e.g., 0.016 for 60 FPS)
      deltaTime = (currentTime - lastTime) / 1000.0f; 
      lastTime = currentTime;



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
          newBullet.dy = -5;
          newBullet.active = true;

          bullets.push_back(newBullet);
          shotCooldown = 0.75f;
        }

        if (shotCooldown > 0) {
          shotCooldown -= 0.016f;  // Minus 16ms per frame assuming 60FPS this equals a quarter second.
        }


        // Color Screen background
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);

        // --- DRAW YOUR GRAPHICS HERE ---
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 825, 1400, 825);

        SDL_Rect PLAYER {playerX, playerY, playerW, playerH};

        SDL_RenderCopy(renderer, playerTexture, NULL, &PLAYER);




        SDL_Surface*  SCORESurface = TTF_RenderText_Blended(font, std::to_string(SCORE).c_str(), textColor);
        SDL_Texture* SCORETexture = SDL_CreateTextureFromSurface(renderer, SCORESurface);

        SDL_Rect SCORErect;
        SCORErect.x = 200;
        SCORErect.y = 50;
        SCORErect.w = SCORESurface->w;
        SCORErect.h = SCORESurface->h;
        SDL_FreeSurface(SCORESurface);


        renderBarriers(renderer, barriers);
        moveEnemies(renderer, imageTexture);


        renderBullets(renderer, bullets);
        updateBullets(bullets);

        shootEnemyBullets(enemyBullets, deltaTime);

        renderBullets(renderer, enemyBullets);
        updateBullets(enemyBullets);

        for (auto& enemyBullet : enemyBullets) {
          if (!enemyBullet.active) continue;

            for (auto& barrier : barriers) {
              if (!barrier.active) continue;
                
              int x1 = barrier.x1;
              int y1 = barrier.y1;
              int x2 = barrier.x2;
              int y2 = barrier.y2;


              if (SDL_IntersectRectAndLine(&enemyBullet.bulletrect, &x1, &y1, &x2, &y2)) {
                enemyBullet.active = false;
              
                barrier.y1 += 10;
                if (barrier.y2 - barrier.y1 <= 0) {
                  barrier.active = false;
                }
                break;



              }
            }
            if (SDL_HasIntersection(&enemyBullet.bulletrect, &PLAYER)) {
              enemyBullet.active = false;
              killPlayer();

            }
        }


        std::erase_if(enemyBullets, [](const Bullet& eb) { return !eb.active; });
        std::erase_if(barriers, [](const Barrier& B) { return !B.active; });
        
        // STEP 1: Detect collisions and flag entities as inactive
        for (auto& bullet : bullets) {
          // If the bullet is already inactive, skip it
          if (!bullet.active) continue; 

            for (auto& barrier : barriers) {
              if (!barrier.active) continue;

              int x1 = barrier.x1;
              int y1 = barrier.y1;
              int x2 = barrier.x2;
              int y2 = barrier.y2;

              if (SDL_IntersectRectAndLine(&bullet.bulletrect, &x1, &y1, &x2, &y2)) {
                bullet.active = false;
                barrier.y2 -= 10;
                if (barrier.y2 - barrier.y1 <= 0) {
                  barrier.active = false;
                }
                break;
              }
            }

          for (auto& instance : instances) {
            // If the enemy instance is already inactive, skip it
            if (!instance.active) continue;
            

            // Check the intersection
            if (SDL_HasIntersection(&instance.destrect, &bullet.bulletrect)) {
              SCORE += 10;
              bullet.active = false;   // Mark bullet as inactive
              instance.active = false; // Mark enemy instance as inactive

              break; // Stop checking this bullet; it already hit something
            }
          }
        }

        // Erase elements where active is false
        std::erase_if(bullets, [](const Bullet& b) { return !b.active; });
        std::erase_if(instances, [](const GameObject& i) { return !i.active; });
        std::erase_if(barriers, [](const Barrier& B) { return !B.active; });


        //Render all Fonts 
        SDL_RenderCopy(renderer, creditsTexture, NULL, &creditsrect);
        SDL_RenderCopy(renderer, CREDITSTexture, NULL, &CREDITSrect);
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scorerect);
        SDL_RenderCopy(renderer, SCORETexture, NULL, &SCORErect);
        SDL_DestroyTexture(SCORETexture);
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

