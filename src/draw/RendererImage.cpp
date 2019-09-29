#include <algorithm>
#include <cstring>
#include <limits>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include "Renderer.h"
#include "ColorUtils.h"
#include "../parser/WaveModel.h"
#include "../logger/Logger.h"

using namespace Velvet;


Uint32 get_pixel_at(Uint32* pixels, int x, int y, int w)
{
   return pixels[y * w + x];
}

void Velvet::Renderer::drawImage( ImagePtr pImage )
{
   auto pSurface = pImage->getSurface();

   if ( !pSurface )
   {
      Logger::error( "Attempt to draw null surface!" );
      return;
   }

   Pixel p = { 0, 255, 255, 255 };

   unsigned int* ptr = (unsigned int*)pSurface->pixels;
   size_t scanlineWidth = sizeof( uint32_t ) * pSurface->w;
   size_t pitchLine = pSurface->pitch / sizeof( uint32_t );

   for ( int y = 0; y <= pSurface->h; ++y )
   {
      
      fastCopyScanline( y, &ptr[y * pitchLine], scanlineWidth );

      /*
      for ( int x = 0; x <= pSurface->w; ++x )
      {
         
         
         //ptr[lineoffset + x] = 0x333333;

         //SDL_Color color = pSurface->format->palette->colors[index];

         p = ptr[lineoffset + x];

         drawPixel( x, y, p );
      }*/
   }

   //memcpy( m_buffer.data(), &pSurface->pixels, (size_t)pSurface->pitch * (size_t)pSurface->h );

   //SDL_RenderCopy( m_sdlRenderer, pImage->getSurface(), NULL, &pImage->getRect() );
}