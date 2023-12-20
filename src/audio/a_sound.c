#include "a_sound.h"

#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

// Function creating new sound. It is expected Mix_OpenAudio() has already been
// called.
A_Sound* a_new_sound(const char* path) {
  A_Sound* a_sound = malloc(sizeof(A_Sound));
  if (a_sound == NULL) {
    printf("[ERROR] Err a_new_sound - a_sound malloc NULL\n");
    return NULL;
  }

  a_sound->music = Mix_LoadMUS(path);
  if (a_sound->music == NULL) {
    printf("[ERROR] Err a_new_sound - loading music failed: %s\n",
           Mix_GetError());
    a_destroy_sound(a_sound);
    return NULL;
  }

  return a_sound;
}

void a_destroy_sound(A_Sound* a_sound) {
  if (a_sound != NULL) {
    if (a_sound->music != NULL) {
      Mix_FreeMusic(a_sound->music);
    }

    free(a_sound);
  }
}

int a_play_sound(A_Sound* a_sound) {
  int ret = Mix_PlayMusic(a_sound->music, 0);
  return ret;
}
