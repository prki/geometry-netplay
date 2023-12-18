# geometry-netplay
:)

## Interesting behavior

Running `valgrind` with configuration to have VSYNC off results in the game seemingly sleeping forever.
This is believed to be caused by a call to SDL_Delay() to implement framerate waiting. Since framerate
capping other than vsync is not supported and not intended to be (major problem - SDL ticks are timed
at milliseconds with the game possibly being updated/rendered faster), VSYNC off is mainly a configuration
item to validate framerate independency.

When testing for memleaks with valgrind, make sure that f_config initialized at the start has vsync on.
