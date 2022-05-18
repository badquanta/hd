list(APPEND LIBHD_COMPONENTS
#  Common.cpp
  Wrap.cpp
  Engine.cpp
  EngineComponent.cpp
  Dispatch.cpp
  Rect.cpp
  Ui.cpp

  sdl/Joystick.cpp
  sdl/GameController.cpp
  sdl/MixerChunk.cpp
  sdl/MixerChannel.cpp
  sdl/Surface.cpp
  sdl/GLContext.cpp
  sdl/Window.cpp
  sdl/Font.cpp
  sdl/Texture.cpp
  sdl/Display.cpp

  sdl/ControllerDeviceDispatch.cpp
  sdl/ControllerDispatch.cpp
  sdl/DropDispatch.cpp
  sdl/EngineDispatch.cpp
  sdl/JoyDeviceDispatch.cpp
  sdl/JoyDispatch.cpp
  sdl/KeyboardDispatch.cpp
  sdl/KeyDispatch.cpp
  sdl/MouseButtonDispatch.cpp
  sdl/MouseDispatch.cpp
  sdl/EventDispatch.cpp
  sdl/TextDispatch.cpp
  sdl/WindowDispatch.cpp
  sdl/WindowIdDispatch.cpp
  sdl/events.cpp

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
#target_precompile_headers(hd PRIVATE ../../include/hd/Common.hpp)
endif()
