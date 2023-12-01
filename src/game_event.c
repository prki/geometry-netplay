#include "game_event.h"

#include <stdio.h>
#include <stdlib.h>

GameEvent* new_game_event(enum GameEventType type) {
  GameEvent* evt = malloc(sizeof(GameEvent));
  if (evt == NULL) {
    printf("[ERROR] malloc error on new_game_event\n");
    return NULL;
  }

  evt->type = type;

  return evt;
}

// Main public function for destroying game events. Based on event type,
// calls private destroy functions related to the type, so that the
// user of an event does not need to call different functions.
void destroy_game_event(GameEvent* evt) {
  if (evt != NULL) {
    if (evt->type == RECT_COLLISION) {
      destroy_rect_collision_data((RectCollisionData*)evt->data);
    }

    free(evt);
  }
}

GameEventQueue* new_game_event_queue(void) {
  GameEventQueue* queue = malloc(sizeof(GameEventQueue));
  if (queue == NULL) {
    printf("[ERROR] malloc error on new_game_event_queue\n");
    return NULL;
  }

  queue->head = NULL;
  queue->tail = NULL;

  return queue;
}

GameEventQueueNode* new_game_event_queue_node(GameEvent* evt) {
  if (evt == NULL) {
    printf("[ERROR] cant create event queue node with null evt\n");
    return NULL;
  }

  GameEventQueueNode* node = malloc(sizeof(GameEventQueueNode));
  if (node == NULL) {
    printf("[ERROR] malloc error new_game_event_queue_node\n");
    return NULL;
  }

  node->evt = evt;
  node->next = NULL;

  return node;
}

void destroy_game_event_queue_node(GameEventQueueNode* node) {
  if (node != NULL) {
    free(node);
  }
}

// Function destroys all contents of the queue, if there are any
void destroy_game_event_queue(GameEventQueue* queue) {
  if (queue != NULL) {
    GameEventQueueNode* node = queue->head;
    while (node != NULL) {
      GameEventQueueNode* next = node->next;
      destroy_game_event_queue_node(node);
      node = next;
    }

    free(queue);
  }
}

GameEvent* evt_queue_dequeue(GameEventQueue* queue) {
  if (queue->head == NULL) {
    return NULL;
  }

  GameEvent* ret = queue->head->evt;
  GameEventQueueNode* tmp = queue->head;
  queue->head = queue->head->next;

  if (queue->head == NULL) {
    queue->tail = NULL;
  }

  free(tmp);

  return ret;
}

void evt_queue_enqueue(GameEventQueue* queue, GameEvent* evt) {
  GameEventQueueNode* node = new_game_event_queue_node(evt);
  if (queue->tail == NULL) {
    queue->head = node;
    queue->tail = node;
  } else {
    queue->tail->next = node;
    queue->tail = node;
  }
}

RectCollisionData* new_rect_collision_data(const Rectangle* r1,
                                           const Rectangle* r2) {
  RectCollisionData* data = malloc(sizeof(RectCollisionData));
  if (data == NULL) {
    printf("[ERROR] Rect collision event data malloc failed\n");
    return NULL;
  }

  data->rect1 = r1;
  data->rect2 = r2;

  return data;
}

// Function does not destroy underlying data. Function
// should also not be called publically, as destruction is handled
// by destroy_game_event(GameEvent* evt)
void destroy_rect_collision_data(RectCollisionData* data) {
  if (data != NULL) {
    free(data);
  }
}

// Utility function creating a new RECT_COLLISION game event.
// This is equivalent to creating a GameEvent with the RECT_COLLISION
// type and assigning RectCollisionData to the data member attr.
GameEvent* new_rect_collision_event(const Rectangle* r1, const Rectangle* r2) {
  GameEvent* evt = new_game_event(RECT_COLLISION);
  if (evt == NULL) {
    return NULL;
  }

  RectCollisionData* data = new_rect_collision_data(r1, r2);
  if (data == NULL) {
    destroy_game_event(evt);
    return NULL;
  }

  evt->data = data;

  return evt;
}

GameEvent* new_bullet_impact_event(const vec2d impact_point) {
  GameEvent* evt = new_game_event(BULLET_IMPACT);
  if (evt == NULL) {
    printf("[ERROR] Error creating a new bullet impact event\n");
    return NULL;
  }

  BulletImpactData* data = malloc(sizeof(BulletImpactData));
  if (data == NULL) {
    printf("[ERROR] malloc error bulletimpactdata\n");
    destroy_game_event(evt);
    return NULL;
  }

  data->impact_point = impact_point;
  evt->data = data;

  return evt;
}
