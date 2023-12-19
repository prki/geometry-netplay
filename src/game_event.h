#ifndef _GAME_EVENT_H_
#define _GAME_EVENT_H_
#include "math/shapes.h"
#include "math/vector.h"

enum GameEventType { SHIP_EXPLOSION, RECT_COLLISION, BULLET_IMPACT };

typedef struct GameEvent {
  enum GameEventType type;
  void* data;
} GameEvent;

GameEvent* new_game_event(enum GameEventType type);
void destroy_game_event(GameEvent* evt);

// [TODO] It's own header/implementation
typedef struct GameEventQueueNode {
  GameEvent* evt;
  struct GameEventQueueNode* next;
} GameEventQueueNode;

GameEventQueueNode* new_game_event_queue_node(GameEvent* evt);

typedef struct GameEventQueue {
  GameEventQueueNode* head;
  GameEventQueueNode* tail;
} GameEventQueue;

GameEventQueue* new_game_event_queue(void);
void destroy_game_event_queue(GameEventQueue* queue);
GameEvent* evt_queue_dequeue(GameEventQueue* queue);
void evt_queue_enqueue(GameEventQueue* queue, GameEvent* evt);

// [TODO] To be implemented once bullets are implemented
typedef struct ShipExplosionData {
} ShipExplosionData;

typedef struct RectCollisionData {
  const Rectangle* rect1;
  const Rectangle* rect2;
} RectCollisionData;

RectCollisionData* new_rect_collision_data(const Rectangle* r1,
                                           const Rectangle* r2);
GameEvent* new_rect_collision_event(const Rectangle* r1, const Rectangle* r2);
void destroy_rect_collision_data(RectCollisionData* evt);

typedef struct BulletImpactData {
  vec2d impact_point;
} BulletImpactData;

GameEvent* new_bullet_impact_event(const vec2d impact_point);
#endif  // _GAME_EVENT_H_
