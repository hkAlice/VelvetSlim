#ifndef _COLORUTILS
#define _COLORUTILS

#include <cmath>

#include "../data/Pixel.h"

namespace Velvet
{
   namespace ColorUtils
   {
        static void sRGBCompandingReverse( Pixel& pixel )
        {
            float r = pixel.r / 255;
            float g = pixel.g / 255;
            float b = pixel.b / 255;

            if( r > 0.04045 )
                r = std::pow( ( r + 0.055 ) / 1.055, 2.4 );
            else
            r = r / 12.92;

            if( g > 0.04045 )
                g = std::pow( ( g + 0.055 ) / 1.055, 2.4 );
            else
                g = g / 12.92;

            if( b > 0.04045 )
                b = std::pow( ( b + 0.055 ) / 1.055, 2.4 );
            else
                b = b / 12.92;


            pixel.r = r * 255;
            pixel.g = g * 255;
            pixel.b = b * 255;
      }

      static void sRGBCompandingAdjust( Pixel& pixel )
      {
            float r = pixel.r / 255;
            float g = pixel.g / 255;
            float b = pixel.b / 255;

            if( r > 0.0031308 )
                r = 1.055 * std::pow( r, 1 / 2.4 ) - 0.055;
            else
                r = r * 12.92;
            if( g > 0.0031308 )
                g = 1.055 * std::pow( g, 1 / 2.4 ) - 0.055;
            else
                g = g * 12.92;
            if( b > 0.0031308 )
                b = 1.055 * std::pow( b, 1 / 2.4 ) - 0.055;
            else
                b = b * 12.92;

            pixel.r = r * 255;
            pixel.g = g * 255;
            pixel.b = b * 255;
      }

      static Pixel randomPixelColor()
      {
        return { static_cast< uint8_t >( rand() % 255 ), 
                 static_cast< uint8_t >( rand() % 255 ), 
                 static_cast< uint8_t >( rand() % 255 ), 
                 255 };
      }
   }
}
#endif // _COLORUTILS