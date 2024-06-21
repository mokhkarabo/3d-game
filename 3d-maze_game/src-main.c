#include <SDL2/SDL.h>
#include <stdio.h>
#include "raycasting.h"

void init_game(Player *player, Map *map, Textures *textures, Enemy enemies[], RainDrop rain_drops[]);
void game_loop(SDL_Window *window, SDL_Renderer *renderer, Player *player, Map *map, Textures *textures, Enemy enemies[], RainDrop rain_drops[]);
void render(SDL_Renderer *renderer, Player *player, Map *map, Textures *textures, Enemy enemies[], RainDrop rain_drops[], int toggle_rain);

int map_data[MAP_WIDTH * MAP_HEIGHT] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 1, 1, 0, 1, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player player = { TILE_SIZE * 1.5, TILE_SIZE * 1.5, 0 };
    Map map = { MAP_WIDTH, MAP_HEIGHT, map_data };
    Textures textures;
    Enemy enemies[NUM_ENEMIES];
    RainDrop rain_drops[RAIN_DROP_COUNT];
    int toggle_rain = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("3D Maze Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    init_game(&player, &map, &textures, enemies, rain_drops);
    game_loop(window, renderer, &player, &map, &textures, enemies, rain_drops);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void init_game(Player *player, Map *map, Textures *textures, Enemy enemies[], RainDrop rain_drops[])
{
    map->data = map_data;

    // Load textures
    SDL_Surface *surface = SDL_LoadBMP("assets/floor.bmp");
    textures->floor = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("assets/ceiling.bmp");
    textures->ceiling = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("assets/weapon.bmp");
    textures->weapon = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("assets/enemy.bmp");
    textures->enemy = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Initialize enemies
    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].x = rand() % (MAP_WIDTH * TILE_SIZE);
        enemies[i].y = rand() % (MAP_HEIGHT * TILE_SIZE);
        enemies[i].active = 1;
    }

    // Initialize rain drops
    for (int i = 0; i < RAIN_DROP_COUNT; i++) {
        rain_drops[i].x = rand() % SCREEN_WIDTH;
        rain_drops[i].y = rand() % SCREEN_HEIGHT;
        rain_drops[i].speed = (rand() % 3) + 2;
    }
}

void game_loop(SDL_Window *window, SDL_Renderer *renderer, Player *player, Map *map, Textures *textures, Enemy enemies[], RainDrop rain_drops[])
{
    SDL_Event e;
    int quit = 0;
    int toggle_rain = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_r) {
                    toggle_rain = !toggle_rain;
                }
            }
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        handle_events(&quit, player, state);
        update_rain(rain_drops, RAIN_DROP_COUNT, toggle_rain);

        render(renderer, player, map, textures, enemies, rain_drops, toggle_rain);
        SDL_Delay(1000 / 60); // Cap the frame rate to 60 FPS
    }
}

void render(SDL_Renderer *renderer, Player *player, Map *map, Textures *textures, Enemy enemies[], RainDrop rain_drops[], int toggle_rain)
{
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, SDL_ALPHA_OPAQUE); // Sky color
    SDL_RenderClear(renderer);

    draw_map(renderer, map);
    draw_player(renderer, player);
    cast_rays(renderer, player, map, textures);
    draw_textures(renderer, textures, player);
    draw_enemies(renderer, enemies, NUM_ENEMIES);
    if (toggle_rain) {
        draw_rain(renderer,
