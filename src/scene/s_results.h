#ifndef _S_RESULTS_H_
#define _S_RESULTS_H_
#include <SDL.h>

#include "../renderer/font.h"
#include "../renderer/text.h"
#include "../ui/ui.h"

// Straightforward implementation for two players.
// The report scoreboard should be sorted for multiple players ([TODO])
typedef struct {
  int score_winner;
  int score_loser;
} ResultReport;

typedef struct {
  R_Text* header_text;
  R_Text* winner_text;
  R_Text* loser_text;
  const R_Font* font;
  UI_Button* rematch_btn;
  UI_Button* menu_btn;
} S_Results;

S_Results* s_new_results(const FontStorage* r_fs, SDL_Renderer* renderer);
void s_destroy_results(S_Results* s_results);
int s_setup_results(S_Results* s_results, const ResultReport result_report,
                    SDL_Renderer* renderer);
void s_render_s_results(S_Results* s_results, SDL_Renderer* renderer);

#endif  // _S_RESULTS_H_
