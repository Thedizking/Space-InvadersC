# Space-InvadersC

A simple Space Invaders clone using C++ and SDL2

## REQUIRED DEPENDENCIES

  - SDL2
  ```
  sudo apt install libsdl2-dev libsdl2-2.0-0
  ```

  - SDL2_ttf
  ```
  sudo apt install libsdl2-ttf-dev
  ```

  - SDL2_image
  ```
  sudo apt-get install libsdl2-image-dev
  ```

  - SDL2_mixer
  ```
  sudo apt install libsdl2-mixer-dev
  ```

## Linux Compilation 

```
g++ main.cpp -o SpaceInv -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -std=c++23
```

##Cmake Build
```
cmake --build ${{github.workspace}}/build --config ${{env.BUILD_TYPE}}
```

