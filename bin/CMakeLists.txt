list(
  APPEND
  LIBHD_COMPONENTS
  hdCommon.cpp
  hdScene.cpp
  hdShared.cpp
  hdSplashScene.cpp
  hdApp.cpp
  hdEvents.cpp)
add_executable(hddemo hd.cpp)
target_link_libraries(hddemo hd)
