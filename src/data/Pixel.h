#ifndef _PIXEL
#define _PIXEL

namespace Velvet
{
   union Pixel
   {
      Pixel()
         : integer(0) { }
      Pixel( uint32_t pixel32 )
         : integer( pixel32 ) { }
      Pixel( const uint32_t& pixel32 )
         : integer( pixel32 ) { }
      Pixel( SDL_Color* sdlColor )
         : b( sdlColor->b ), g( sdlColor->g ), r( sdlColor->r ) { }

      Pixel( uint8_t bi, uint8_t gi, uint8_t ri, uint8_t ai = 255 )
      {
         b = bi;
         g = gi;
         r = ri;
         a = ai;
      }

      uint32_t integer;

      struct
      {
         uint8_t a, r, g, b;
      };

   };

}

#endif // _PIXEL