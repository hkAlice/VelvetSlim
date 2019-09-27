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
         uint8_t b, g, r, a;
      };

   };

}

#endif // _PIXEL