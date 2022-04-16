add_executable(hd hd.cpp hdCommon.cpp hdScene.cpp hdShared.cpp hdSplashScene.cpp hdApp.cpp hdEvents.cpp)
target_link_libraries(hd SDL2::Main SDL2::Image SDL2::TTF SDL2::Net SDL2::Mixer SDL2::GFX)