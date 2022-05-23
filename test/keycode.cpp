#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <hd/sdl/events.hpp>
// Demonstrate some basic assertions.
TEST (keycode, getsCalled)
{
  testing::MockFunction<void (const SDL_Event &e)> test;
  hd::sdl::EngineDispatch list;
  list.Windows[123].Key.Keycode[SDLK_0].On(test.AsStdFunction());

  SDL_Event e;
  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_1;
  e.key.windowID = 123;
  EXPECT_CALL (test, Call (testing::_)).Times (1);
  list.Trigger(e);
  e.key.keysym.sym = SDLK_0;
  list.Trigger(e);
}