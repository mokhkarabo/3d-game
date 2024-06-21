#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define TILE_SIZE 64
#define FOV (60 * (M_PI / 180))
#define MOVE_SPEED 2
#define ROTATE_SPEED 0.05
#define NUM_ENEMIES 5
#define RAIN_DROP_COUNT 100

typedef struct {
    float x, y;
    float angle;
} Player;

typedef struct {
    int width, height;
    int *data;
} Map;

typedef struct {
    float x, y;
    int active;
} Enemy;

typedef struct {
    SDL_Texture *floor;
    SDL_Texture *ceiling;
    SDL_Texture *weapon;
    SDL_Texture *enemy;
} Textures;

typedef struct {
    float x, y;
    float speed;
} RainDrop;

void draw_map(SDL_Renderer *renderer, Map *map);
void draw_player(SDL_Renderer *renderer, Player *player);
void cast_rays(SDL_Renderer *renderer, Player *player, Map *map, Textures *textures);
void handle_events(int *quit, Player *player, const Uint8 *state);
void draw_textures(SDL_Renderer *renderer, Textures *textures, Player *player);
void draw_enemies(SDL_Renderer *renderer, Enemy enemies[], int num_enemies);
void draw_rain(SDL_Renderer *renderer, RainDrop rain_drops[], int count);
void update_rain(RainDrop rain_drops[], int count, int toggle_rain);

#endif /* RAYCASTING_H */
