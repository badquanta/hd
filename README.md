# hd
You have reached the `hd` [repository](#TODO). Here you will find the sources of the project. You can [checkout & build](#TODO) the project yourself with [cmake](#cmake).
Perhaps someday in the near future you can [install](#TODO) it from a [release](#TODO). [Thank you](#credits) to the many people who make this possible.

# About
hd is just a placeholder name for the project.
It means something to me; not "high definition"; but I'll probably name it something else later.

I'm not certain what it will be.
Maybe a game; maybe a library for games. Maybe both.

## Todo

This shouldn't be a comprehensive list; lots of `todo` items will be defined elsewhere.
However if I took the time to document it in the `README.md` it means it's either important; or pertains to something I need to work on within the `README.md` itself.

- README.md
    - repository information (checkout instructions, url, branches, conventions, etc.)
    - building instructions
    - runtime & build dependencies
    - CREDITSs
## git repository
The main/`origin` repository resides on [github](https://github.com/badquanta/hd).
- [src/](https://github.com/badquanta/hd/tree/development/src)
    Mostly `.cpp` and `.hpp` files that define `hd`.
- [docs/](https://github.com/badquanta/hd/tree/development/docs)
    - [CMakeLists.txt](https://github.com/badquanta/hd/blob/development/docs/CMakeLists.txt)
- [patterns/](https://github.com/badquanta/hd/tree/development/patterns)
    Asset files for the "game."
- [build/](#TODO)
    This is not normally revision controlled; but may be so one day in a release-candidate [branch](#branches).

### branches

- [development](https://github.com/badquanta/hd/tree/development/) may or may not compile.
- [release-candidate](#TODO) should always compile & run. __TODO__

### Dependencies
#### Build
<a name='cmake'>
### cmake

#### Runtime
<a name='credits'>
## Credits

A lot of open source software was utilized just to get a skeleton running:

- [cmake](https://cmake.org/)
    - https://cmake.org/cmake/help/latest/module/FindDoxygen.html
    - modules
        - https://github.com/aminosbh/sdl2-cmake-modules
- [doxygen](https://www.doxygen.nl/)
    - https://www.doxygen.nl/manual/config.html
    - https://www.doxygen.nl/manual/commands.html
- c libraries
    - [SDL2](#TODO)
        - [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
        - [SDL2_GFX](#TODO)
        - [SDL2_TTF](https://github.com/libsdl-org/SDL_ttf)
        - [SDL2_Mixer](https://www.libsdl.org/projects/SDL_mixer/)
        - [SDL2_Net](https://www.libsdl.org/projects/SDL_net/)
        - https://www.libsdl.org/projects/SDL_rtf/
    - [eventpp](https://github.com/wqking/eventpp)


### random useful links
- https://en.cppreference.com/w/cpp/links/libs
- https://www.boost.org/doc/libs/