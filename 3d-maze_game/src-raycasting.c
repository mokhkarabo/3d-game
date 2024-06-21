#include "raycasting.h"
#include <math.h>
#include <SDL2/SDL.h>

void draw_map(SDL_Renderer *renderer, Map *map)
{
    int x, y, tile;

    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            tile = map->data[y * map->width + x];
            if (tile == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(renderer, 169, 169, 169, SDL_ALPHA_OPAQUE);
            }
            SDL_Rect rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void draw_player(SDL_Renderer *renderer, Player *player)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = { player->x - 5, player->y - 5, 10, 10 };
    SDL_RenderFillRect(renderer, &rect);
}

void cast_rays(SDL_Renderer *renderer, Player *player, Map *map)
{
    int i;
    float ray_angle, dist_to_wall;
    int hit_wall;
    int wall_x, wall_y;

    for (i = 0; i < SCREEN_WIDTH; i++) {
        ray_angle = (player->angle - FOV / 2.0) + ((float)i / (float)SCREEN_WIDTH) * FOV;
        dist_to_wall = 0;
        hit_wall = 0;

        while (!hit_wall && dist_to_wall < 20) {
            dist_to_wall += 0.1;

            wall_x = (int)(player->x + cos(ray_angle) * dist_to_wall);
            wall_y = (int)(player->y + sin(ray_angle) * dist_to_wall);

            if (wall_x < 0 || wall_x >= MAP_WIDTH * TILE_SIZE || wall_y < 0 || wall_y >= MAP_HEIGHT * TILE_SIZE) {
                hit_wall = 1;
                dist_to_wall = 20;
            } else if (map->data[(wall_y / TILE_SIZE) * map->width + (wall_x / TILE_SIZE)] == 1) {
                hit_wall = 1;
            }
        }

        int line_height = (int)(SCREEN_HEIGHT / dist_to_wall);
        int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0) {
            draw_start = 0;
        }
        int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_end >= SCREEN_HEIGHT) {
            draw_end = SCREEN_HEIGHT - 1;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE); // Wall color
        SDL_RenderDrawLine(renderer, i, draw_start, i, draw_end);

        SDL_SetRenderDrawColor(renderer, 169, 169, 169, SDL_ALPHA_OPAQUE); // Floor color
        SDL_RenderDrawLine(renderer, i, draw_end, i, SCREEN_HEIGHT
