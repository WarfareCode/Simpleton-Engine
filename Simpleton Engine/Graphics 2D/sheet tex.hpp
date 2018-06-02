//
//  sheet tex.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/5/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_sheet_tex_hpp
#define engine_graphics_2d_sheet_tex_hpp

#include "renderer.hpp"
#include "../Sprite/sheet.hpp"

namespace G2D {
  /// A Sprite::Sheet and a TextureID
  class SheetTex {
  public:
    void load(Renderer &, const std::string &, GLenum = GL_NEAREST);
    const Sprite::Sheet &sheet() const;
    TextureID tex() const;
    
  private:
    Sprite::Sheet sheet_;
    TextureID tex_;
  };
}

#include "sheet tex.inl"

#endif
