list(APPEND LIBHD_COMPONENTS Common.cpp Scene.cpp Shared.cpp SplashScene.cpp Engine.cpp Events.cpp glTexture.cpp glProgram.cpp glShader.cpp glVAO.cpp glVBO.cpp glEBO.cpp)
#add_executable(hddemo hd.cpp ${LIBHD_COMPONENTS})
add_library(hd ${LIBHD_COMPONENTS} )
#target_link_libraries(hddemo SDL2::Main SDL2::Image SDL2::TTF SDL2::Net SDL2::Mixer SDL2::GFX)
target_link_libraries(hd SDL2::Core SDL2::Image SDL2::TTF SDL2::Net SDL2::Mixer SDL2::GFX OpenGL::GLU OpenGL::OpenGL OpenGL::EGL GLEW::GLEW)
target_include_directories(hd
  PUBLIC
  "${PROJECT_BINARY_DIR}/include/"
  "${PROJECT_SOURCE_DIR}/include/"
  "${PROJECT_SOURCE_DIR}/ext/eventpp/include"
  "${PROJECT_SOURCE_DIR}/ext/glm")
target_precompile_headers(hd PRIVATE ../../include/hd/Common.hpp)