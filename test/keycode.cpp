#include <gtest/gtest.h>

#include <hdEvents.hpp>
// Demonstrate some basic assertions.
TEST (keycode, getsCalled)
{
  hd::sdl::event::List list;
  hd::sdl::event::TypeDispatcher type;
  hd::sdl::event::key::CodeDispatcher keyDispatcher;
  //list.append (type);
  list.append ([&type] (const SDL_Event &e) { type.dispatch (e); });
  type.appendListener (SDL_KEYDOWN,
                       [] (const SDL_Event &e) { printf ("KEYDOWN\n"); });
  //printf ("SDL_KEYDOWN = %d\n", SDL_KEYDOWN);
  auto forward
      = [&keyDispatcher] (const SDL_Event &e) { keyDispatcher.dispatch (e); };
  type.appendListener (SDL_KEYDOWN, forward);
  type.appendListener (SDL_KEYUP, forward);
  bool called = false;
  keyDispatcher.appendListener (
      SDLK_0, [&called] (const SDL_Event &e) { called = true; });
  SDL_Event e;
  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_1;
  list(e);
  EXPECT_EQ (called, false);
  e.key.keysym.sym = SDLK_0;
  list(e);
  EXPECT_EQ (called, true);
}