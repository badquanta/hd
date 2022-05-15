# README.md

You have reached the `hd` [repository](#repository).
Here you will find the sources of the project and more [about](#about) it.
You can [checkout & build](#checkout_and_build) the project yourself with [cmake](#cmake).
Perhaps someday in the near future you can [install](#TODO) it from a [release](#TODO).
[Thank you](#credits) to the many people who make this possible.

# About
hd is just a placeholder name for the project.

I'm not certain what it will be.
Maybe a game; maybe a library for games. Maybe both. It's fun.

<a name="repository">&nbsp;</a>

# git repository

The `origin` repository resides on [github][PROJECT_GITHUB].
- [include/hd/](https://github.com/badquanta/hd/tree/development/include/hd)
    - [gl/](https://github.com/badquanta/hd/tree/development/include/hd/gl)
        OpenGL interface.
    - [evt/](https://github.com/badquanta/hd/tree/development/include/hd/evt)
        "Event Dispatchers."  Includes a dispatcher series for `SDL_Event`s and a couple of generic versions of this dispatcher.
- [src/hd/](https://github.com/badquanta/hd/tree/development/src/hd)
    Implementation.
- [bin/](https://github.com/badquanta/hd/tree/development/bin)
    integration testing and demonstrations. the `hd*` executables are following my own take on
- [test/](https://github.com/badquanta/hd/tree/development/test)
- [docs/](https://github.com/badquanta/hd/tree/development/docs)
    - [CMakeLists.txt](https://github.com/badquanta/hd/blob/development/docs/CMakeLists.txt)
- [assets/](https://github.com/badquanta/hd/tree/development/assets)
    Asset files for the "game."
- [build/](#TODO)
    This is not normally revision controlled; but may be so one day in a release-candidate [branch](#branches).

## branches

- [development](https://github.com/badquanta/hd/tree/development/) may or may not compile.
- [release-candidate](#TODO) should always compile & run. __TODO__

<a name="dependencies">&nbsp;</a>

# Dependencies

In order to build one will need at a minimum the development libraries are available:
    - sdl, sdl_ttf, sdl_img, sdl_mixer, sdl_net (etc)
    - boost::program_options 1.74
    - __@TODO__ document more / test that am not missing any others on a clean install.

The repository contains some other dependencies as git submodules:
    - [sdl2 cmake modules](https://gitlab.com/aminosbh/sdl2-cmake-modules)
    - [glm](https://github.com/g-truc/glm.git)

In order to generate a build environment you will need [cmake][CMAKE_HOME]

<a name="checkout_and_build">&nbsp;</a>

# Checkout & Build environment

Use git to _recursively_ clone the repository. Then create a build directory, generate a build with cmake and then build it. __example:__

            git clone https://github.com/badquanta/hd.git --recursive
            mkdir build-hd
            cd build-hd
            cmake ../hd
            cmake --build ./


<a name='cmake'>&nbsp;</a>

# cmake
## configuration options

Aside from the ones already defined (or required) by cmake these are:

- __HD_BUILD_BIN__ _ON / OFF_

    Enable building demonstration binaries.

- __HD_BUILD_TESTS__ _ON / OFF_

    Enable or disable building unit tests.

- __HD_DEBUG_BUILD__ _ON / OFF_

    Enable debugging output; traces code execution: basically print line debugging.

- __HD_COMPILE_COMMON_HPP__ _ON / OFF_

    Successive build times can be shortened by pre-compiling `hd/Common.hpp`.

## cpack

Utilized to generate debian and other installers. Simply execute `cpack` within the build directory.

<a name='credits'>

# Credits

## Assets

The assets here are for testing & prototyping right now. If I did not tag a source properly, please create an [issue][NEW_ISSUE].

- audio
    - [horror ambient.ogg](https://opengameart.org/content/horror-ambient) by [Vladislav Krotov](https://opengameart.org/users/vinrax)
    - [jrpg-pack-1-exploration](https://opengameart.org/content/jrpg-pack-1-exploration)
by [Juhani Junkala](https://opengameart.org/users/subspaceaudio)
    -
- textures
    - pattern_*
Provided by Nobiax / yughues. """Absolutely free to use or to modify in any kind of work (personal, commercial or else)."""
        - [@deviantart](nobiax.deviantart.com)
        - [@OpenGameArt](https://opengameart.org/users/yughues)



## Software

A lot of open source software was utilized just to get a skeleton running:

- [cmake][CMAKE_HOME]
    - https://cmake.org/cmake/help/latest/module/FindDoxygen.html
    - modules
        - https://github.com/aminosbh/sdl2-cmake-modules
- [doxygen][DOXYGEN_HOME]
    - https://www.doxygen.nl/manual/config.html
    - https://www.doxygen.nl/manual/commands.html
- c libraries
    - [SDL2][SDL_HOME]
        - [SDL2_image][SDL_IMAGE_HOME][docs][SDL_IMAGE_DOCS]
        - [SDL2_TTF][SDL_TTF_HOME] [docs][SDL_TTF_DOCS]
        - [SDL2_Mixer][SDL_MIXER_HOME]
        - [SDL2_Net][SDL_NET_HOME]
        - https://www.libsdl.org/projects/SDL_rtf/
- tutorials
    - [Victor Gordan's OpenGL tutorials repository][TUT_VICTOR_REPO] & [youtube playlist][TUT_VICTOR_YOUTUBE].

# random useful links
- https://en.cppreference.com/w/cpp/links/libs
- https://www.boost.org/doc/libs/

<a name="TODO">&nbsp;</a>

# Todo

This shouldn't be a comprehensive list; lots of `todo` items will be defined elsewhere.
However if I took the time to document it in the `README.md` it means it's either important; or pertains to something I need to work on within the `README.md` itself.

- README.md
    - repository information (checkout instructions, url, branches, conventions, etc.)
    - building instructions
    - runtime & build dependencies
    - CREDITSs


[PROJECT_GITHUB]: https://github.com/badquanta/hd
[CMAKE_HOME]: https://cmake.org/
[DOXYGEN_HOME]: https://www.doxygen.nl/
[SDL_HOME]: #TODO
[SDL_IMAGE_HOME]: https://www.libsdl.org/projects/SDL_image/
[SDL_IMAGE_DOCS]: https://www.libsdl.org/projects/docs/SDL_image/SDL_image.html
[SDL_TTF_HOME]: https://github.com/libsdl-org/SDL_ttf
[SDL_TTF_DOCS]: https://www.libsdl.org/projects/docs/SDL_ttf/
[SDL_MIXER_HOME]: https://www.libsdl.org/projects/SDL_mixer/
[SDL_MIXER_DOCS]: https://www.libsdl.org/projects/docs/SDL_mixer/
[SDL_NET_HOME]: https://www.libsdl.org/projects/SDL_net/
[TUT_VICTOR_REPO]: https://github.com/VictorGordan/opengl-tutorials
[TUT_VICTOR_YOUTUBE]: https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-
[NEW_ISSUE]: https://github.com/badquanta/hd/issues/new
[std::shared_ptr]: https://en.cppreference.com/w/cpp/memory/shared_ptr
[SDL_PROJECTS_DOC]: https://www.libsdl.org/projects/docs/