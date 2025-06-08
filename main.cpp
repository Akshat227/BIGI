#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


int width = 200;
int height = width;
int x_position = (650 - width)/2;
int y_position = (600 - height)/2;
double angle = 0.0; // initial angle




SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer)
{
    SDL_Surface *surface = IMG_Load(path);

    if (!surface)
    {
        std::cerr << "[ERROR] Failed to load image at " << path << ": " << IMG_GetError() << "\n";
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        std::cerr << "[ERROR] Failed to create texture from " << path << ": " << SDL_GetError() << "\n";
        return nullptr;
    }

    std::cout << "[INFO] Loaded texture: " << path << "\n";
    return texture;
}

int main(int argc, char *argv[])
{

    SDL_Window *window = SDL_CreateWindow("BIGI",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          650, 600, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* background = loadTexture("assets/background.png", renderer);
    SDL_Texture* object = loadTexture("assets/object.png", renderer);
    SDL_Rect objectRect = {x_position,y_position , width, height};
   
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
        return 1;
    }
    
std::cout << "[INFO] Font loaded:"<<"\n";
TTF_Font* font = TTF_OpenFont("assets/fonts/PressStart.ttf", 35);
SDL_Color white = {255, 255, 255};
SDL_Surface* textSurface = TTF_RenderText_Blended(font, "BluePrint Supreme", white);
SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
SDL_Rect textRect = {20, 20, textSurface->w, textSurface->h};
SDL_FreeSurface(textSurface);





textRect.x = 650 - textRect.w - 20; // 20 px padding from right edge
textRect.y = 600 - textRect.h - 20; // 20 px padding from bottom edge
textRect.x = (650 - textRect.w) / 2;
textRect.y = (600 - textRect.h) / 6;


    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }




       // Rendering time!
SDL_RenderClear(renderer);

//First the background
SDL_RenderCopy(renderer, background, NULL, NULL);

// Then the text on top
SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

 SDL_Point center = { width / 2, height / 2 }; // rotate around center
 SDL_RenderCopyEx(renderer, object, NULL, &objectRect, angle, &center, SDL_FLIP_NONE);

SDL_RenderPresent(renderer);

angle += 0.1; 
    if (angle >= 360.0) angle -= 360.0;
    }

    SDL_DestroyTexture(object);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}