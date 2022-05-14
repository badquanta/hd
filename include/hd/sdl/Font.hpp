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
  /** @see https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf.html **/
  class Font : public WRAP_PTR<TTF_Font> {
  public:
    using WRAP_PTR::WRAP_PTR;
    static Font Load (std::filesystem::path, int);

  public: // Instance methods
    bool GlyphMetrics (Uint16, int *, int *, int *, int *, int *);
    void SetStyle (int);
    int GetStyle ();
    bool IsStyleBold ();
    bool IsStyleItalic ();
    bool IsStyleUnderline ();
    bool IsStyleNormal ();
    int GetHeight ();
    int GetAscent ();
    int GetDescent ();
    int GetLineSkip ();
    long GetFaces ();
    int IsFixedWidth ();
    std::string GetFamilyName ();
    std::string GetStyleName ();
    bool SizeText (std::string, int *, int *);
    bool SizeUTF8 (std::string, int *, int *);
    bool SizeUTF16 (std::u16string, int *, int *);

    Surface RenderSolidText (std::string, SDL_Color);
    Surface RenderSolidUTF8 (std::string, SDL_Color);
    Surface RenderSolidUTF16 (std::u16string, SDL_Color);
    Surface RenderSolidGlyph (Uint16, SDL_Color);

    Surface RenderShadedText (std::string, SDL_Color, SDL_Color);
    Surface RenderShadedUTF8 (std::string, SDL_Color, SDL_Color);
    Surface RenderShadedUTF16 (std::u16string, SDL_Color, SDL_Color);
    Surface RenderShadedGlyph (Uint16, SDL_Color, SDL_Color);

    Surface RenderBlendedText (std::string, SDL_Color);
    Surface RenderBlendedUTF8 (std::string, SDL_Color);
    Surface RenderBlendedUTF16 (std::u16string, SDL_Color);
    Surface RenderBlendedGlyph (Uint16, SDL_Color);

    Surface RenderBlendedTextWrapped (std::string, SDL_Color,Uint32);
    Surface RenderBlendedUTF8Wrapped (std::string, SDL_Color,Uint32);
    Surface RenderBlendedUTF16Wrapped (std::u16string, SDL_Color,Uint32);
  };
}