/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; // The images that correspond to a keypress
SDL_Surface* gCurrentSurface = NULL; // Current displayed image

int main(int argc, char* args[]) {
    
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
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;
                        }
                    }
                }
                
                // Apply the image
                SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
                
                // Update the surface
                SDL_UpdateWindowSurface(gWindow);
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
    
    // Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("04_key_presses/press.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }
    
    // Load up surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("04_key_presses/up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Failed to load up image!\n");
        success = false;
    }
    
    // Load down surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("04_key_presses/down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Failed to load down image!\n");
        success = false;
    }
    
    // Load left surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("04_key_presses/left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Failed to load left image!\n");
        success = false;
    }
    
    // Load right surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("04_key_presses/right.bmp");
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
