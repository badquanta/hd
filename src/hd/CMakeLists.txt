list(APPEND LIBHD_COMPONENTS
  Common.cpp
  Window.cpp
  Scene.cpp
  Shared.cpp
  SplashScene.cpp
  Engine.cpp
  EngineComponent.cpp
  sdl/Joystick.cpp
  sdl/GameController.cpp
  sdl/MixerChunk.cpp
  sdl/MixerChannel.cpp

  evt/AbstractDispatch.cpp
  evt/ControllerDeviceDispatch.cpp
  evt/ControllerDispatch.cpp
  evt/DropDispatch.cpp
  evt/EngineDispatch.cpp
  evt/IntDispatch.cpp
  evt/JoyDeviceDispatch.cpp
  evt/JoyDispatch.cpp
  evt/KeyboardDispatch.cpp
  evt/KeyDispatch.cpp
  evt/MouseButtonDispatch.cpp
  evt/MouseDispatch.cpp
  evt/SDL_EventDispatch.cpp
  evt/TextDispatch.cpp
  evt/VoidDispatch.cpp
  evt/WindowDispatch.cpp
  evt/WindowIdDispatch.cpp

  gl/Camera.cpp
  gl/Texture.cpp
  gl/ShaderProgram.cpp
  gl/Shader.cpp
  gl/VAO.cpp
  gl/VBO.cpp
  gl/EBO.cpp

)
#add_executable(hddemo hd.cpp ${LIBHD_COMPONENTS})
add_library(hd ${LIBHD_COMPONENTS} )
#target_link_libraries(hddemo SDL2::Main SDL2::Image SDL2::TTF SDL2::Net SDL2::Mixer SDL2::GFX)
target_link_libraries(hd Boost::program_options SDL2::Core SDL2::Image SDL2::TTF SDL2::Net SDL2::Mixer SDL2::GFX OpenGL::GLU OpenGL::OpenGL OpenGL::EGL GLEW::GLEW)
target_include_directories(hd
  PUBLIC
  "${PROJECT_BINARY_DIR}/include/"
  "${PROJECT_SOURCE_DIR}/include/"
  "${PROJECT_SOURCE_DIR}/ext/eventpp/include"
  "${PROJECT_SOURCE_DIR}/ext/glm")

if(HD_COMPILE_COMMON_HPP)
target_precompile_headers(hd PRIVATE ../../include/hd/Common.hpp)
endif()