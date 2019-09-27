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

void Velvet::Renderer::drawImage( ImagePtr pImage )
{
   auto pSurface = pImage->getSurface();

   if ( !pSurface )
   {
      Logger::error( "Attempt to draw null surface!" );
      return;
   }

   Pixel p = { 0, 255, 255, 255 };

   for ( int y = 0; y < pSurface->h; ++y )
   {
      for ( int x = 0; x < pSurface->w; ++x )
      {
         const int offset = ( pSurface->h * y ) + x;
         unsigned int* ptr = ( unsigned int* )pSurface->pixels;
         int lineoffset = y * ( pSurface->pitch / 4 );
         
         p = ptr[lineoffset + x];

         drawPixel( x, y, p );
      }
   }
   //memcpy( m_buffer.data(), &pSurface->pixels, (size_t)pSurface->pitch * (size_t)pSurface->h );

   //SDL_RenderCopy( m_sdlRenderer, pImage->getSurface(), NULL, &pImage->getRect() );
}