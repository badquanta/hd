/**
 * @file Font.hpp
 * @author Jón Davíð Sawyer (badquanta@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-05-11
 *
 * @copyright GNU-GPL 3.0
 *
 */
#include "hd/sdl/Surface.hpp"
namespace hd::sdl {
  /**
   * Encapsulates a `std::shared_ptr` instance of a `TTF_Font`
   * @see https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf.html
   ***/
  class Font : public WRAP_PTR<TTF_Font> {
  public:
    /** Inherit constructors  **/
    using WRAP_PTR::WRAP_PTR;
    /**
     * @brief Read a font from filesystem path
     * @return Font the font loaded, will evalute to false if there was an
     * error.
     */
    static Font Load (std::filesystem::path, int);

  public: // Instance methods
    bool GlyphMetrics (Uint16, int *, int *, int *, int *, int *) const;
    void SetStyle (int) const;
    int GetStyle () const;
    bool IsStyleBold () const;
    bool IsStyleItalic () const;
    bool IsStyleUnderline () const;
    bool IsStyleNormal () const;
    int GetHeight () const;
    int GetAscent () const;
    int GetDescent () const;
    int GetLineSkip () const;
    long GetFaces () const;
    int IsFixedWidth () const;
    std::string GetFamilyName () const;
    std::string GetStyleName () const;
    bool SizeText (std::string, int *, int *) const;
    bool SizeUTF8 (std::string, int *, int *) const;
    bool SizeUTF16 (std::u16string, int *, int *) const;

    Surface RenderSolidText (std::string, SDL_Color) const;
    Surface RenderSolidUTF8 (std::string, SDL_Color) const;
    Surface RenderSolidUTF16 (std::u16string, SDL_Color) const;
    Surface RenderSolidGlyph (Uint16, SDL_Color) const;

    Surface RenderShadedText (std::string, SDL_Color, SDL_Color) const;
    Surface RenderShadedUTF8 (std::string, SDL_Color, SDL_Color) const;
    Surface RenderShadedUTF16 (std::u16string, SDL_Color, SDL_Color) const;
    Surface RenderShadedGlyph (Uint16, SDL_Color, SDL_Color) const;

    Surface RenderBlendedText (std::string, SDL_Color) const;
    Surface RenderBlendedUTF8 (std::string, SDL_Color) const;
    Surface RenderBlendedUTF16 (std::u16string, SDL_Color) const;
    Surface RenderBlendedGlyph (Uint16, SDL_Color) const;

    Surface RenderBlendedTextWrapped (std::string, SDL_Color, Uint32) const;
    Surface RenderBlendedUTF8Wrapped (std::string, SDL_Color, Uint32) const;
    Surface
        RenderBlendedUTF16Wrapped (std::u16string, SDL_Color, Uint32) const;
  };
  /**
   * @example fontViewer.cpp
   * Demonstrates how Fonts are loaded & rendered to a surface.
   *
   */
}