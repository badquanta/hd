#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include <hd/sdl/Window.hpp>
using namespace hd;


// Demonstrate some basic assertions.
TEST (sdlWindowTest, CreateIntInt)
{
  // works?
  sdl::Window win = sdl::Window::Create (123, 456);
  int w, h;
  win.GetSize (&w, &h);
  assert (w = 123);
  assert (h = 456);
  // Everything should auto destruct now.
}
