/**
 * @file Font.cpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-13
 *
 * @copyright GNU-GPL 3.0
 *
 */
#include "hd/sdl/Font.hpp"

#include "hd/Error.hpp"
namespace hd::sdl {
  /**
   * @brief
   *
   * @param aPath
   * @param aSize
   * @return Font
   */
  Font
  Font::Load (std::filesystem::path aPath, int aSize)
  {
    return s_ptr (
        TTF_OpenFont (Engine::FindPath (aPath).generic_string ().c_str (),
                      aSize),
        TTF_CloseFont);
  }
  /**
   * @brief
   * @see https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf.html#SEC31
   * @param aChar
   * @param aMinX
   * @param aMinY
   * @param aMaxX
   * @param aMaxY
   * @param aAdvance
   * @return true
   * @return false
   */
  bool
  Font::GlyphMetrics (Uint16 aChar,
                      int *aMinX,
                      int *aMinY,
                      int *aMaxX,
                      int *aMaxY,
                      int *aAdvance)
  {
    assert (*this);
    return TTF_GlyphMetrics (
               *this, aChar, aMinX, aMinY, aMaxX, aMaxY, aAdvance)
           == 0;
  }
  /**
   * @brief
   * @see https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf.html#SEC22
   * @param aStyle
   */
  void
  Font::SetStyle (int aStyle)
  {
    assert (*this);
    TTF_SetFontStyle (*this, aStyle);
  }
  /**
   * @brief
   *
   * @return int
   */
  int
  Font::GetStyle ()
  {
    assert (*this);
    return TTF_GetFontStyle (*this);
  }
  /**
   * @brief
   *
   * @return true
   * @return false
   */
  bool
  Font::IsStyleBold ()
  {
    return (GetStyle () & TTF_STYLE_BOLD);
  }
  /**
   * @brief
   *
   * @return true
   * @return false
   */
  bool
  Font::IsStyleItalic ()
  {
    return (GetStyle () & TTF_STYLE_ITALIC);
  }
  /**
   * @brief
   *
   * @return true
   * @return false
   */
  bool
  Font::IsStyleUnderline ()
  {
    return (GetStyle () & TTF_STYLE_UNDERLINE);
  }
  /**
   * @brief
   *
   * @return true
   * @return false
   */
  bool
  Font::IsStyleNormal ()
  {
    return (GetStyle () == TTF_STYLE_NORMAL);
  }
  /**
   * @brief
   *
   * @return int
   */
  int
  Font::GetHeight ()
  {
    assert (*this);
    return TTF_FontHeight (*this);
  }
  /**
   * @brief
   *
   * @return int
   */
  int
  Font::GetAscent ()
  {
    assert (*this);
    return TTF_FontAscent (*this);
  }
  /**
   * @brief
   *
   * @return int
   */
  int
  Font::GetDescent ()
  {
    assert (*this);
    return TTF_FontDescent (*this);
  }
  /**
   * @brief
   *
   * @return int
   */
  int
  Font::GetLineSkip ()
  {
    assert (*this);
    return TTF_FontLineSkip (*this);
  }
  /**
   * @brief
   *
   * @return long
   */
  long
  Font::GetFaces ()
  {
    assert (*this);
    return TTF_FontFaces (*this);
  }
  /**
   * @brief
   *
   * @return int
   */
  int
  Font::IsFixedWidth ()
  {
    assert (*this);
    return TTF_FontFaceIsFixedWidth (*this);
  }

  std::string
  Font::GetFamilyName ()
  {
    assert (*this);
    return TTF_FontFaceFamilyName (*this);
  }
  std::string
  Font::GetStyleName ()
  {
    assert (*this);
    return TTF_FontFaceStyleName (*this);
  }

  bool
  Font::SizeText (std::string aString, int *aWidth, int *aHeight)
  {
    return TTF_SizeText (*this, aString.c_str (), aWidth, aHeight) == 0;
  }
  bool
  Font::SizeUTF8 (std::string aString, int *aWidth, int *aHeight)
  {
    return TTF_SizeUTF8 (*this, aString.c_str (), aWidth, aHeight) == 0;
  }
  bool
  Font::SizeUTF16 (std::u16string aString, int *aWidth, int *aHeight)
  {
    return TTF_SizeUNICODE (*this, (Uint16 *)aString.c_str (), aWidth, aHeight)
           == 0;
  }

  Surface
  Font::RenderSolidText (std::string aString, SDL_Color aColor)
  {
    return Surface::Create (
        TTF_RenderText_Solid (*this, aString.c_str (), aColor), true);
  }

  Surface
  Font::RenderSolidUTF8 (std::string aString, SDL_Color aColor)
  {
    return Surface::Create (
        TTF_RenderUTF8_Solid (*this, aString.c_str (), aColor), true);
  }

  Surface
  Font::RenderSolidUTF16 (std::u16string aString, SDL_Color aColor)
  {
    return Surface::Create (
        TTF_RenderUNICODE_Solid (*this, (Uint16 *)aString.c_str (), aColor),
        true);
  }

  Surface
  Font::RenderSolidGlyph (Uint16 aChar, SDL_Color aColor)
  {
    return Surface::Create (TTF_RenderGlyph_Solid (*this, aChar, aColor),
                            true);
  }

  Surface
  Font::RenderShadedText (std::string aText, SDL_Color aFg, SDL_Color aBg)
  {
    return Surface::Create (
        TTF_RenderText_Shaded (*this, aText.c_str (), aFg, aBg), true);
  }

  Surface
  Font::RenderShadedUTF8 (std::string aText, SDL_Color aFg, SDL_Color aBg)
  {
    return Surface::Create (
        TTF_RenderUTF8_Shaded (*this, aText.c_str (), aFg, aBg), true);
  }

  Surface
  Font::RenderShadedUTF16 (std::u16string aText, SDL_Color aFg, SDL_Color aBg)
  {
    return Surface::Create (
        TTF_RenderUNICODE_Shaded (*this, (Uint16 *)aText.c_str (), aFg, aBg),
        true);
  }

  Surface
  Font::RenderShadedGlyph (Uint16 aText, SDL_Color aFg, SDL_Color aBg)
  {
    return Surface::Create (TTF_RenderGlyph_Shaded (*this, aText, aFg, aBg),
                            true);
  }

  Surface
  Font::RenderBlendedText (std::string aText, SDL_Color aFg)
  {
    return Surface::Create (
        TTF_RenderText_Blended (*this, aText.c_str (), aFg), true);
  }

  Surface
  Font::RenderBlendedUTF8 (std::string aText, SDL_Color aFg)
  {
    return Surface::Create (
        TTF_RenderUTF8_Blended (*this, aText.c_str (), aFg), true);
  }

  Surface
  Font::RenderBlendedUTF16 (std::u16string aText, SDL_Color aFg)
  {
    return Surface::Create (
        TTF_RenderUNICODE_Blended (*this, (Uint16 *)aText.c_str (), aFg),
        true);
  }

  Surface
  Font::RenderBlendedGlyph (Uint16 aText, SDL_Color aFg)
  {
    return Surface::Create (TTF_RenderGlyph_Blended (*this, aText, aFg), true);
  }

  Surface
  Font::RenderBlendedTextWrapped (std::string aText,
                                  SDL_Color aFg,
                                  Uint32 aWrapAt)
  {
    return Surface::Create (
        TTF_RenderText_Blended_Wrapped (*this, aText.c_str (), aFg, aWrapAt),
        true);
  }
  Surface
  Font::RenderBlendedUTF8Wrapped (std::string aText,
                                  SDL_Color aFg,
                                  Uint32 aWrapAt)
  {
    return Surface::Create (
        TTF_RenderUTF8_Blended_Wrapped (*this, aText.c_str (), aFg, aWrapAt),
        true);
  }

  Surface
  Font::RenderBlendedUTF16Wrapped (std::u16string aText,
                                   SDL_Color aFg,
                                   Uint32 aWrapAt)
  {
    return Surface::Create (TTF_RenderUNICODE_Blended_Wrapped (
                                *this, (Uint16 *)aText.c_str (), aFg, aWrapAt),
                            true);
  }
}