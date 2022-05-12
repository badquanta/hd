#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <hd/Wrap.hpp>

class IntPtr : public hd::WRAP_PTR<int> {
public:
  using WRAP_PTR::WRAP_PTR;
  using WRAP_PTR::operator=;
};

// Demonstrate some basic assertions.
TEST (wraptest, canWrapInt)
{
  hd::WrapInt myInt = 21;
  int something = 0;
  assert (myInt == 21);
  something = myInt;
  assert (something == 21);
}

// Demonstrate the PTR version
TEST (wrapPtrTest, canWrapPtrInt)
{
  IntPtr myIntPtr;
  myIntPtr = IntPtr::s_ptr (new int);
  IntPtr::s_ptr anotherInt (new int);
  IntPtr anotherPtr;
  anotherPtr = myIntPtr;
  anotherPtr = anotherInt;
  assert (myIntPtr == true); // this just means it is pointing to SOMETHING.

}