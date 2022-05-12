#include "hd/gl/Texture.hpp"
#include "hd/sdl/Surface.hpp"
namespace hd::gl {
  bool Texture::Create(std::filesystem::path aPath, GLenum aType, GLenum aSlot){
    type = aType;
    sdl::Surface image = sdl::Surface::Load (aPath);
    if(image.ptr == NULL){
      fprintf (stderr,
               "Unable to load image from path: '%s'\n",
               aPath.generic_string ().c_str ());
      return false;
    }
    sdl::Surface::s_ptr converted = image.Convert ();
    glGenTextures (1, &ID);
    glActiveTexture (aSlot);
    glBindTexture (type, ID);
    glTexParameteri (type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D (type,
                  0,
                  GL_RGBA,
                  converted->w,
                  converted->h,
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  converted->pixels);
    glGenerateMipmap (type);
    glBindTexture (type, 0);
    image.m_IDENTITY = NULL;
    converted = NULL;
    return true;
  }

  void Texture::Assign(ShaderProgram&aProgram,const char*aUniform,GLuint aUnit){
    GLint texUni = aProgram.getUniformLocation (aUniform);
    aProgram.Bind ();
    glUniform1i (texUni, aUnit);
  }

  void Texture::Bind(){
    glBindTexture (type, ID);
  }

  void Texture::Unbind(){
    glBindTexture (type, 0);
  }

  void Texture::Delete(){
    glDeleteTextures (1, &ID);
  }
}