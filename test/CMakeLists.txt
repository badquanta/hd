
add_executable(keycode keycode.cpp)
target_link_libraries(keycode hd gtest_main)
add_test(keycode keycode)
#include(GoogleTest)
#gtest_discover_tests(keycode)