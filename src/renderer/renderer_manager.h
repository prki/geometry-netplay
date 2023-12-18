#ifndef _RENDERER_MANAGER_H
#define _RENDERER_MANAGER_H
#include <SDL.h>

#include "../game.h"
#include "../math/shapes.h"
#include "../player.h"
#include "../scene/s_main_menu.h"
#include "../scene/s_results.h"
#include "hud.h"
#include "particle.h"

#define MAX_RENDERABLE_PLAYERS 32

// [TODO] Consider separating renderer manager into its own folder and struct
// defs
// [TODO] Actually, the code seems to converge to the architectural idea of
// having scenes and orchestrating those scenes - in such case, r_mngr would not
// hold such information itself.

//////////////////////////////
// Entities
//////////////////////////////

typedef struct RenderablePlayer {
  Player* player;  // Player is not owned
  SDL_Texture* texture;
} RenderablePlayer;

void destroy_renderable_player(RenderablePlayer* r_plr);

typedef struct RenderableBulletPool {
  BulletPool* bullet_pool;
  SDL_Texture* texture;
} RenderableBulletPool;

//////////////////////////////
// Manager
//////////////////////////////

// [TODO] Move game-related pieces to a "game scene"
typedef struct RendererManager {
  SDL_Renderer* renderer;

  RenderablePlayer renderable_players[MAX_RENDERABLE_PLAYERS];
  size_t renderable_players_size;
  RenderableBulletPool bullet_pool;
  ParticlePool* particle_pool;
  GameEventQueue* game_event_queue;
  GameWorld* game_world;  // [TODO] Grow from game_world to include renderable
                          // rectangles with different textures etc
  FontStorage* font_storage;
  R_HUD* hud;
} RendererManager;

RendererManager* new_renderer_manager(SDL_Window* window,
                                      SDL_Renderer* renderer);
void destroy_renderer_manager(RendererManager* r_mngr);
int register_game(RendererManager* r_mngr, Game* game);
int register_player(RendererManager* r_mngr, Player* plr);
void draw_player(RendererManager* r_mngr, RenderablePlayer* r_plr);
void draw_rectangle_outline(RendererManager* r_mngr, const Rectangle* rect);
int r_load_assets_fonts(RendererManager* r_mngr);

void render_frame(RendererManager* r_mngr, double delta_time);
void r_render_s_main_menu(RendererManager* r_mngr, S_Main_Menu* s_menu);
void r_render_s_results(RendererManager* r_mngr, S_Results* s_results);
void r_display_frame(RendererManager* r_mngr);

#endif  // _RENDERER_MANAGER_H
