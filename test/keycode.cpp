#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <hd/Events.hpp>
// Demonstrate some basic assertions.
TEST (keycode, getsCalled)
{
  testing::MockFunction<void (const SDL_Event &e)> test;
  hd::sdl::event::Tree list;
  list.keyCode.appendListener (SDLK_0, test.AsStdFunction());

  SDL_Event e;
  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_1;
  EXPECT_CALL (test, Call (testing::_)).Times(1);
  list(e);
  e.key.keysym.sym = SDLK_0;
  list(e);
}