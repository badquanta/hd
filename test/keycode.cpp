#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <hd/evt/EngineDispatch.hpp>
// Demonstrate some basic assertions.
TEST (keycode, getsCalled)
{
  testing::MockFunction<void (const SDL_Event &e)> test;
  hd::evt::EngineDispatch list;
  list.Windows[123].Key.Keycode[SDLK_0].Add(test.AsStdFunction());

  SDL_Event e;
  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_1;
  e.key.windowID = 123;
  EXPECT_CALL (test, Call (testing::_)).Times (1);
  list.Trigger(e);
  e.key.keysym.sym = SDLK_0;
  list.Trigger(e);
}