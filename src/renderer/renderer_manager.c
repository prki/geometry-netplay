#include "renderer_manager.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "../asset_loader.h"
#include "../game.h"
#include "../game_world.h"
#include "../math/vector.h"
#include "hud.h"
#include "particle.h"

// [TODO] We probably want to move these to particle.[c|h]
#define MAX_PARTICLE_VELOCITY 200
#define MAX_PARTICLE_ADD_TIME_TO_LIVE 400
#define MIN_PARTICLE_ADD_TIME_TO_LIVE 5
#define MAX_FONTSTORAGE 16

void _nullify_renderable_players(RendererManager* r_mngr) {
  for (size_t i = 0; i < MAX_RENDERABLE_PLAYERS; i++) {
    r_mngr->renderable_players[i].player = NULL;
    r_mngr->renderable_players[i].texture = NULL;
  }
}

// [TODO] Create a better alloc/free flow
RendererManager* new_renderer_manager(SDL_Window* window,
                                      SDL_Renderer* renderer) {
  if (renderer == NULL) {
    printf("[ERROR] Cannot initialize r_manager - NULL SDL_Renderer passed\n");
    return NULL;
  }
  if (window == NULL) {
    printf("[ERROR] Cannot initialize r_manager - NULL SDL_Window passed\n");
    return NULL;
  }
  if (TTF_Init() < 0) {
    printf("[ERROR] Cannot initialize TTF by r_manager\n");
    return NULL;
  }

  RendererManager* mngr = malloc(sizeof(RendererManager));
  if (mngr == NULL) {
    TTF_Quit();
    return NULL;
  }
  ParticlePool* particle_pool = new_particle_pool();
  if (particle_pool == NULL) {
    printf("[ERROR] Cannot initialize r_manager - NULL particle pool\n");
    TTF_Quit();
    free(mngr);
    return NULL;
  }
  FontStorage* fs = r_new_font_storage(MAX_FONTSTORAGE);
  if (fs == NULL) {
    printf("[ERROR] err init r_manager - NULL fontstorage\n");
    TTF_Quit();
    free(mngr);
    destroy_particle_pool(particle_pool);
    return NULL;
  }

  R_HUD* hud = new_r_hud(fs);
  if (hud == NULL) {
    printf("[ERROR] err init r_manager - NULL r_hud\n");
    TTF_Quit();
    free(mngr);
    destroy_particle_pool(particle_pool);
    r_destroy_font_storage(fs);
    return NULL;
  }

  mngr->renderer = renderer;
  mngr->particle_pool = particle_pool;
  mngr->hud = hud;

  _nullify_renderable_players(mngr);
  mngr->renderable_players_size = 0;

  mngr->game_event_queue = NULL;
  mngr->font_storage = fs;

  return mngr;
}

/* Function to be used internally for freeing renderable resources
 * of a player. Note that the player resource is not freed, as
 * the manager does not own the player.*/
void destroy_renderable_player(RenderablePlayer* r_plr) {
  if (r_plr != NULL) {
    if (r_plr->texture != NULL) {
      SDL_DestroyTexture(r_plr->texture);
    }
  }
}

/* Function freeing resources of renderer manager, to be used
 * externally. Handles all other resources owned.*/
void destroy_renderer_manager(RendererManager* r_mngr) {
  if (r_mngr != NULL) {
    for (size_t i = 0; i < MAX_RENDERABLE_PLAYERS; i++) {
      destroy_renderable_player(&(r_mngr->renderable_players[i]));
    }
    if (r_mngr->particle_pool != NULL) {
      destroy_particle_pool(r_mngr->particle_pool);
    }
    if (r_mngr->hud != NULL) {
      destroy_r_hud(r_mngr->hud);
    }
    if (r_mngr->font_storage != NULL) {
      r_destroy_font_storage(r_mngr->font_storage);
    }

    SDL_DestroyRenderer(r_mngr->renderer);
    TTF_Quit();
    free(r_mngr);
  }
}

// Function loading font assets used by the game.
// Returns 1 on success, 0 on failure. Assumes TTF_Init()
// has already been called.
int r_load_assets_fonts(RendererManager* r_mngr) {
  R_Font* font = new_font("./assets/fonts/Arial.ttf", 12);
  if (font == NULL) {
    printf("[ERROR] err loading assets (fonts) - font load failure\n");
    return 0;
  }

  int succ = r_insert_new_font(r_mngr->font_storage, font);
  if (!succ) {
    printf("[ERROR] err loading assets (fonts) - font insert failure\n");
    return 0;
  }

  return 1;
}

int register_player(RendererManager* r_mngr, Player* plr) {
  SDL_Texture* plr_txtr = NULL;
  if (plr == NULL) {
    return 0;
  }
  if (r_mngr->renderable_players_size >= MAX_RENDERABLE_PLAYERS) {
    printf("[WARN] r_mngr cant register player - renderable size > max\n");
    return 0;
  }

  plr_txtr =
      load_texture_from_bmp(r_mngr->renderer, "./assets/player_texture.bmp");
  if (plr_txtr == NULL) {
    printf("[ERROR] Error loading player texture bitmap during registration\n");
    return 0;
  }

  r_mngr->renderable_players[r_mngr->renderable_players_size].player = plr;
  r_mngr->renderable_players[r_mngr->renderable_players_size].texture =
      plr_txtr;

  r_mngr->renderable_players_size = r_mngr->renderable_players_size + 1;

  return 1;
}

// [TODO] Assign bullet texture once bullet asset is created
int register_bullet_pool(RendererManager* r_mngr, BulletPool* bullet_pool) {
  if (bullet_pool == NULL) {
    return 0;
  }

  r_mngr->bullet_pool.bullet_pool = bullet_pool;
  r_mngr->bullet_pool.texture = NULL;

  return 1;
}

int register_game_world(RendererManager* r_mngr, GameWorld* world) {
  if (world == NULL) {
    printf("[ERROR] Attempted to register rects but rect_arr was NULL\n");
    return 0;
  }

  r_mngr->game_world = world;

  return 1;
}

int register_event_queue(RendererManager* r_mngr, GameEventQueue* queue) {
  if (queue == NULL) {
    printf("[ERROR] Attempted to register event queue but queue was NULL\n");
    return 0;
  }

  r_mngr->game_event_queue = queue;

  return 1;
}

int register_game(RendererManager* r_mngr, Game* game) {
  int succ = 0;

  for (size_t i = 0; i < G_MAX_PC_PLAYERS; i++) {
    if (game->players[i] != NULL) {
      succ = register_player(r_mngr, game->players[i]);
      if (!succ) {
        printf("[ERROR] Error registering player into renderer manager\n");
        return 0;
      }
    }
  }

  succ = register_game_world(r_mngr, game->game_world);
  if (!succ) {
    printf("[ERROR] Error registering rects into renderer manager\n");
    return 0;
  }

  succ = register_event_queue(r_mngr, game->evt_queue);
  if (!succ) {
    printf("[ERROR] Error registering event queue into renderer manager\n");
    return 0;
  }

  succ = register_bullet_pool(r_mngr, game->bullet_pool);
  if (!succ) {
    printf("[ERROR] Error registering bullet pool into renderer manager\n");
    return 0;
  }

  return 1;
}

/* [TODO] Validate SDL_RenderCopy return code*/
void draw_player(RendererManager* r_mngr, RenderablePlayer* r_plr) {
  SDL_Rect draw_rect;
  draw_rect.h = r_plr->player->player_ship.ship_width;
  draw_rect.w = r_plr->player->player_ship.ship_width;
  draw_rect.x = (r_plr->player->player_ship.position.x);
  draw_rect.y = (r_plr->player->player_ship.position.y);

  double rotation_angle = 0;
  vec2d north = {.x = 0, .y = -1};
  const vec2d shoot_direction = r_plr->player->shoot_direction;

  rotation_angle = vec2d_angle(&north, &shoot_direction);
  if (shoot_direction.x < 0) {
    rotation_angle = 180 + (180 - rotation_angle);
  }

  SDL_RenderCopyEx(r_mngr->renderer, r_plr->texture, NULL, &draw_rect,
                   rotation_angle, NULL, 0);
}

// Implementation of midpoint circle algorithm, see:
// https://en.wikipedia.org/w/index.php?title=Midpoint_circle_algorithm&oldid=889172082#C_example
// Note the function draws 8 points each step. It might be better to collect
// all points and draw then. [OPTIMIZATION]
void draw_circle_outline(RendererManager* r_mngr, const Circle* circ) {
  // int x = circ.position.x;
  int x = circ->radius - 1;
  int y = 0;
  int dx = 1;
  int dy = 1;
  int err = dx - (circ->radius * 2);

  while (x >= y) {
    SDL_Point points[8];
    points[0] =
        (SDL_Point){.x = circ->position.x + x, .y = circ->position.y + y};
    points[1] =
        (SDL_Point){.x = circ->position.x + y, .y = circ->position.y + x};
    points[2] =
        (SDL_Point){.x = circ->position.x - y, .y = circ->position.y + x};
    points[3] =
        (SDL_Point){.x = circ->position.x - x, .y = circ->position.y + y};
    points[4] =
        (SDL_Point){.x = circ->position.x - x, .y = circ->position.y - y};
    points[5] =
        (SDL_Point){.x = circ->position.x - y, .y = circ->position.y - x};
    points[6] =
        (SDL_Point){.x = circ->position.x + y, .y = circ->position.y - x};
    points[7] =
        (SDL_Point){.x = circ->position.x + x, .y = circ->position.y - y};

    if (err <= 0) {
      y++;
      err += dy;
      dy += 2;
    }

    if (err > 0) {
      x--;
      dx += 2;
      err += dx - (circ->radius * 2);
    }

    SDL_RenderDrawPoints(r_mngr->renderer, points, 8);
  }
}

void draw_rectangle_outline(RendererManager* r_mngr, const Rectangle* rect) {
  // Naming of nodes follows comment of Rectangle struct in math/shapes.h
  // A1->A2
  SDL_RenderDrawLine(r_mngr->renderer, rect->a1.x, rect->a1.y, rect->a2.x,
                     rect->a2.y);
  // A1->A3
  SDL_RenderDrawLine(r_mngr->renderer, rect->a1.x, rect->a1.y, rect->a3.x,
                     rect->a3.y);
  // A2->A4
  SDL_RenderDrawLine(r_mngr->renderer, rect->a2.x, rect->a2.y, rect->a4.x,
                     rect->a4.y);
  // A3->A4
  SDL_RenderDrawLine(r_mngr->renderer, rect->a3.x, rect->a3.y, rect->a4.x,
                     rect->a4.y);
}

void draw_rectangles(RendererManager* r_mngr) {
  SDL_SetRenderDrawColor(r_mngr->renderer, 255, 0, 0, 255);
  for (unsigned int i = 0; i < r_mngr->game_world->rects->size; i++) {
    draw_rectangle_outline(r_mngr, &(r_mngr->game_world->rects->array[i]));
  }
}

void draw_player_outlines(RendererManager* r_mngr, RenderablePlayer* r_plr) {
  // Draw collision circle
  SDL_SetRenderDrawColor(r_mngr->renderer, 0, 0, 255, 255);
  draw_circle_outline(r_mngr, &r_plr->player->player_ship.collision_circ);
  // Draw hurt circle
  SDL_SetRenderDrawColor(r_mngr->renderer, 0, 255, 0, 255);
  draw_circle_outline(r_mngr, &r_plr->player->player_ship.hurtcirc);

  // Draw "texture origin" point
  SDL_RenderDrawPoint(r_mngr->renderer, r_plr->player->player_ship.position.x,
                      r_plr->player->player_ship.position.y);
}

void render_bullets(RendererManager* r_mngr) {
  SDL_SetRenderDrawColor(r_mngr->renderer, 0, 255, 0, 255);
  for (unsigned int i = 0; i < r_mngr->bullet_pool.bullet_pool->pool_size;
       i++) {
    const Bullet* tmp = &(r_mngr->bullet_pool.bullet_pool->bullets[i]);
    if (tmp->is_active) {
      draw_rectangle_outline(r_mngr, &tmp->hitbox);
    }
  }
}

// [TODO] Should not only take impact point but also the impact line vector
// => that way, it will be possible to make a half circle, rather than all
// particles. But let's first generate around a circle and see how it looks
// around edges - it might also be worth it to introduce collision detection.
void _generate_particle(ParticlePool* particle_pool, const vec2d impact_point,
                        double delta_time) {
  Particle* particle = get_inactive_particle(particle_pool);
  if (particle == NULL) {
    printf("[WARN] Particle pool full, could not get inactive particle\n");
    return;
  }
  size_t explosion_radius = 1;
  double initial_x = ((double)rand() / RAND_MAX) * 2 - 1;
  double initial_y = ((double)rand() / RAND_MAX) * 2 - 1;
  vec2d initial_direction = {.x = initial_x, .y = initial_y};
  initial_direction = vec2d_normalize(&initial_direction);

  double rand_distance = ((double)rand() / RAND_MAX) * explosion_radius;
  vec2d initial_position = {
      .x = impact_point.x + initial_direction.x * rand_distance,
      .y = impact_point.y + initial_direction.y * rand_distance};
  double particle_velocity =
      ((double)rand() / RAND_MAX) * MAX_PARTICLE_VELOCITY;

  particle->velocity = particle_velocity * delta_time;
  particle->direction = initial_direction;
  enable_particle(particle);
  particle->time_to_live = PARTICLE_TIME_TO_LIVE;
  double decay_rate = MIN_PARTICLE_ADD_TIME_TO_LIVE +
                      (rand() % (MAX_PARTICLE_ADD_TIME_TO_LIVE -
                                 MIN_PARTICLE_ADD_TIME_TO_LIVE));
  particle->time_to_live += decay_rate;
  particle->time_alive = 0;
  particle->position = initial_position;
}

// [TODO] Once more players are introduced, collect the particles into points so
// that one SDL call is performed and not an SDL call per point.
void render_particles(RendererManager* r_mngr) {
  SDL_SetRenderDrawColor(r_mngr->renderer, 255, 255, 255, 255);
  for (size_t i = 0; i < r_mngr->particle_pool->pool_size; i++) {
    const Particle* tmp = &r_mngr->particle_pool->particles[i];
    if (tmp->is_active) {
      SDL_RenderDrawPoint(r_mngr->renderer, tmp->position.x, tmp->position.y);
    }
  }
}

// Function rendering all game-related elements into the SDL renderer.
// Note that it does not call SDL_RenderPresent() as there may be post-rendering
// activities to perform, such as rendering scoreboards or anything else
// (considered "metadata"). r_display_frame() should be called once the frame
// should be rendered in its entirety.
void render_frame(RendererManager* r_mngr, double delta_time) {
  SDL_SetRenderDrawColor(r_mngr->renderer, 0, 0, 0, 255);
  SDL_RenderClear(r_mngr->renderer);

  for (size_t i = 0; i < r_mngr->renderable_players_size; i++) {
    draw_player(r_mngr, &r_mngr->renderable_players[i]);
    draw_player_outlines(r_mngr, &r_mngr->renderable_players[i]);
  }
  SDL_SetRenderDrawColor(r_mngr->renderer, 255, 0, 0, 255);

  draw_rectangles(r_mngr);

  GameEvent* evt = NULL;
  while ((evt = evt_queue_dequeue(r_mngr->game_event_queue)) != NULL) {
    if (evt->type == RECT_COLLISION) {
      printf("[INFO] [RENDERER] Game event: Collision!\n");
    } else if (evt->type == BULLET_IMPACT) {
      BulletImpactData* impact_data = (BulletImpactData*)(evt->data);
      for (size_t i = 0; i < 100; i++) {
        _generate_particle(r_mngr->particle_pool, impact_data->impact_point,
                           delta_time);
      }
    }
    destroy_game_event(evt);
  }

  render_bullets(r_mngr);
  update_particles(r_mngr->particle_pool, delta_time);
  render_particles(r_mngr);
}

void r_display_frame(RendererManager* r_mngr) {
  SDL_RenderPresent(r_mngr->renderer);
}
