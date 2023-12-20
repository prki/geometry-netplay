#ifndef _A_SOUND_H_
#define _A_SOUND_H_
#include <SDL_mixer.h>

// Even though SDL_mixer's API looks very straight-forward, we create a wrapper
// for extensibility. We'll see how that goes.
// Currently, this is a wrapper over Mix_Music. However, the mixer can play only
// one music file at once - likely not efficient for sound effects, which would
// need to be mixed together. For now, we can keep it trivial but we'll need to
// explore further.
typedef struct {
  Mix_Music* music;
} A_Sound;

A_Sound* a_new_sound(const char* path);
void a_destroy_sound(A_Sound* a_sound);
int a_play_sound(A_Sound* a_sound);

#endif  // _A_SOUND_H_
