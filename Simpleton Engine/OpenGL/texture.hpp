//
//  texture.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_texture_hpp
#define engine_opengl_texture_hpp

#include "opengl.hpp"
#include <glm/vec2.hpp>
#include "generic raii.hpp"

namespace GL {
  namespace detail {
    inline void deleteTexture(const GLuint &id) {
      glDeleteTextures(1, &id);
    }
  }

  class Texture {
  public:
    RAII_CLASS_MEMBERS(Texture, GLuint, id, detail::deleteTexture)
  
    void bind(GLenum) const;
  
  private:
    GLuint id;
  };
  
  struct TexParams2D {
    GLint wrapS = GL_REPEAT;
    GLint wrapT = GL_REPEAT;
    GLint minFilter = GL_NEAREST_MIPMAP_LINEAR;
    GLint magFilter = GL_LINEAR;
    
    void setWrap(GLint);
    void setFilter(GLint);
  };
  
  struct Image2D {
    const void *data = nullptr;
    GLsizei width = 0;
    GLsizei height = 0;
    GLint pitch = 0;
    bool alpha = true;
  };
  
  Texture makeTexture2D(const Image2D &, const TexParams2D &);
}

#include "texture.inl"

#endif
