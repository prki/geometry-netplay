# geometry-netplay
:)

## Building

This is a project developed in C, provided with a CMakeLists.txt file for simplifying building on different systems. Dependencies are fetched via CMake's Fetch_Content() function from Github.

Therefore, the following can be executed, or an equivalent thereof, based on the OS:
```
mkdir build
cd build
cmake ..
cmake . --build
```

Dependencies required on the building machine:

  * Git
  * SDL dependencies - refer to [SDL installation](https://wiki.libsdl.org/SDL2/Installation). Note that as this project builds SDL, only the dependencies are required. On Windows, from anecdotal experience, nothing needs to be done. On Linux, it might be necessary to install certain packages. The project could also be built with pre-installed SDL on machine, but the CMakeLists.txt configuration would need to be modified.

Note that assets required to run the game are not provided, or at least not all of them. Where you get them I don't know.

## Interesting behavior

Running `valgrind` with configuration to have VSYNC off results in the game seemingly sleeping forever.
This is believed to be caused by a call to SDL_Delay() to implement framerate waiting. Since framerate
capping other than vsync is not supported and not intended to be (major problem - SDL ticks are timed
at milliseconds with the game possibly being updated/rendered faster), VSYNC off is mainly a configuration
item to validate framerate independency.

When testing for memleaks with valgrind, make sure that f_config initialized at the start has vsync on.
