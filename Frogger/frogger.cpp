/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <array>

// Screen dimension constants
const int SCREEN_WIDTH = 432;
const int SCREEN_HEIGHT = 512;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 13;

/*
 * Enums
 */
enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

/*
 * Function prototypes
 */
bool init(); // Prototype to start up SDL and create window
bool loadMedia(); // Prototype to load media
void close(); // Prototype to free media and shut down SDL
SDL_Surface* loadSurface(std::string path); // Loads individual image

/*
 * Global variables
 */
SDL_Window* gWindow = NULL; // The window we'll be rendering to
SDL_Surface* gScreenSurface = NULL; // The surface contained by the window
SDL_Surface* backgroundSurface = NULL; // The surface that will hold the background
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; // The images that correspond to a keypress
SDL_Surface* gCurrentSurface = NULL; // Current displayed image
SDL_Surface* lilypadSurface[9]; // Lilypad images
SDL_Surface* wonSurface = NULL; // Congratulations screen
SDL_Surface* loseSurface = NULL; // Gameover screen
SDL_Rect froggerRect; // Frogger rect
SDL_Rect lilypadRect[9]; // Lilypad rects
std::array< std::array<int, 10>, 13> froggerMap;
int froggerColumn = 5;
int froggerRow = 12;


int main(int argc, char* args[]) {
    // Initialize froggerMap
    froggerMap = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    // Initialize froggerRect
    froggerRect.x = (SCREEN_WIDTH / 10) * froggerColumn;
    froggerRect.y = (SCREEN_HEIGHT / 13) * froggerRow + 5;
    froggerRect.h = 1;
    froggerRect.w = 1;
    
    // Initialize lilypadRect(s)
    for (int i = 0; i < 9; i++) {
        lilypadRect[i].h = 1;
        lilypadRect[i].w = 1;
    }
    // Lilypad 0
    lilypadRect[0].x = (SCREEN_WIDTH / 10) * 4 + 4;
    lilypadRect[0].y = (SCREEN_HEIGHT / 13) * 5 + 5;
    // Lilypad 1
    lilypadRect[1].x = (SCREEN_WIDTH / 10) * 4 + 4;
    lilypadRect[1].y = (SCREEN_HEIGHT / 13) * 4 + 5;
    // Lilypad 2
    lilypadRect[2].x = (SCREEN_WIDTH / 10) * 3 + 8;
    lilypadRect[2].y = (SCREEN_HEIGHT / 13) * 4 + 5;
    // Lilypad 3
    lilypadRect[3].x = (SCREEN_WIDTH / 10) * 3 + 8;
    lilypadRect[3].y = (SCREEN_HEIGHT / 13) * 3 + 5;
    // Lilypad 4
    lilypadRect[4].x = (SCREEN_WIDTH / 10) * 3 + 8;
    lilypadRect[4].y = (SCREEN_HEIGHT / 13) * 2 + 5;
    // Lilypad 5
    lilypadRect[5].x = (SCREEN_WIDTH / 10) * 4 + 4;
    lilypadRect[5].y = (SCREEN_HEIGHT / 13) * 2 + 5;
    // Lilypad 6
    lilypadRect[6].x = (SCREEN_WIDTH / 10) * 5;
    lilypadRect[6].y = (SCREEN_HEIGHT / 13) * 2 + 5;
    // Lilypad 7
    lilypadRect[7].x = (SCREEN_WIDTH / 10) * 6 - 4;
    lilypadRect[7].y = (SCREEN_HEIGHT / 13) * 2 + 5;
    // Lilypad 8
    lilypadRect[8].x = (SCREEN_WIDTH / 10) * 6 - 4;
    lilypadRect[8].y = (SCREEN_HEIGHT / 13) * 1 + 5;
    
    // Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        // Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        }
        else {
            // Main loop flag
            bool quit = false;
            
            // Won flag
            bool won = false;
            
            // Lose flag
            bool lose = false;
            
            // Event handler
            SDL_Event e;
            
            // Set default current surface
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            
            // Event loop
            while (!quit){
                // Handle events on queue
                while (SDL_PollEvent(&e)){
                    // User requests quit
                    if (e.type == SDL_QUIT){
                        quit = true;
                    }
                    // User presses a key
                    else if (e.type == SDL_KEYDOWN) {
                        // Select surface based on key press
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                if (froggerRow > 0) {
                                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                    froggerRect.y -= 39;
                                    froggerRow--;
                                }
                                break;
                            case SDLK_DOWN:
                                if (froggerRow < 12) {
                                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                    froggerRect.y += 39;
                                    froggerRow++;
                                }
                                break;
                            case SDLK_LEFT:
                                if (froggerColumn > 0) {
                                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                    froggerRect.x -= 39;
                                    froggerColumn--;
                                }
                                break;
                            case SDLK_RIGHT:
                                if (froggerColumn < 9) {
                                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                    froggerRect.x += 39;
                                    froggerColumn++;
                                }
                                break;
                            case SDLK_q:
                                quit = true;
                                break;
                            default:
                                break;
                        }
                        printf("XPos: %d,\tYPos: %d\n", froggerColumn, froggerRow);
                    }
                    // Check collision
                    if (froggerMap[froggerRow][froggerColumn] == 1) {
                        lose = true;
                        quit = true;
                    }
                    else if (froggerMap[froggerRow][froggerColumn] == 2) {
                        won = true;
                        quit = true;
                    }
                    
                }
                
                if (lose) {
                    // Clear screen
                    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                    
                    // Gameover screen
                    SDL_BlitSurface(loseSurface, NULL, gScreenSurface, NULL);
                    
                    // Update the surface
                    SDL_UpdateWindowSurface(gWindow);
                    
                    // Delay 2 seconds
                    SDL_Delay(2000);
                }
                
                if (!won) {
                    // Apply the images
                    SDL_BlitSurface(backgroundSurface, NULL, gScreenSurface, NULL);
                    for (int i = 0; i < 9; i++) {
                        SDL_BlitSurface(lilypadSurface[i], NULL, gScreenSurface, &lilypadRect[i]);
                    }
                    SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, &froggerRect);
                    
                    // Update the surface
                    SDL_UpdateWindowSurface(gWindow);
                }
                else {
                    // Clear screen
                    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                    
                    // Congrats screen
                    SDL_BlitSurface(wonSurface, NULL, gScreenSurface, NULL);
                    
                    // Update the surface
                    SDL_UpdateWindowSurface(gWindow);
                    
                    // Delay 2 seconds
                    SDL_Delay(2000);
                }
            }
        }
    }
    
    // Free resources and close SDL
    close();
    
	return 0;
    
}

// Function to start up SDL and create window
bool init() {
    // Initialization flag
    bool success = true;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    
    return success;
}

// Function to load media
bool loadMedia() {
    // Loading success flag
    bool success = true;
    
    // Load won surface
    wonSurface = loadSurface("Images/frogger_win.bmp");
    if (wonSurface == NULL) {
        printf("Failed to load win surface!\n");
        success = false;
    }
    
    // Load gameover surface
    loseSurface = loadSurface("Images/frogger_gameover.bmp");
    if (loseSurface == NULL) {
        printf("Failed to load lose surface!\n");
        success = false;
    }
    
    // Load background surface
    backgroundSurface = loadSurface("Images/frogger_background.bmp");
    if (backgroundSurface == NULL) {
        printf("Failed to load background surface!\n");
        success = false;
    }
    
    // Load lilypad surfaces
    for (int i = 0; i < 9; i++) {
        lilypadSurface[i] = loadSurface("Images/frogger_lilypad.bmp");
        if (lilypadSurface[i] == NULL) {
            printf("Failed to load lilypad #%d!\n", i);
            success = false;
        }
    }
    
    // Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Images/frogger_looking_up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }
    
    // Load up surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("Images/frogger_looking_up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Failed to load up image!\n");
        success = false;
    }
    
    // Load down surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("Images/frogger_looking_down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Failed to load down image!\n");
        success = false;
    }
    
    // Load left surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("Images/frogger_looking_left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Failed to load left image!\n");
        success = false;
    }
    
    // Load right surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Images/frogger_looking_right.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("Failed to load right image!\n");
        success = false;
    }
    
    return success;
}

// Function to free media and shut down SDL
void close() {
    // Deallocate surface
    SDL_FreeSurface(gScreenSurface);
    SDL_FreeSurface(gCurrentSurface);
    gScreenSurface = NULL; // The surface contained by the window
    gCurrentSurface = NULL; // Current displayed image
    
    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    
    // Quit SDL subsystems
    SDL_Quit();
}

// Function to load individual image
SDL_Surface* loadSurface(std::string path) {
    // Final optimized image
    SDL_Surface* optimizedSurface = NULL;
    
    // Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    
    if (loadedSurface == NULL) {
        printf("Unable to load image %s: SDL_Error: %s\n", path.c_str(), SDL_GetError());
    }
    else {
        // Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s: SDL_Error: %s\n", path.c_str(), SDL_GetError());
        }
        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    
    return optimizedSurface;
}
