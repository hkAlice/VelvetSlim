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

#include "../experimental/fastVU.h"

using namespace Velvet;

// HACK: Really, MSVC??
#undef min
#undef max

static uint64_t frameCount = 0;

/* 
   TODO: More research on why pixel format 
   affects performance this much
   At 4000x2480:
   SDL_PIXELFORMAT_ABGR8888 - 10 FPS
   SDL_PIXELFORMAT_ARGB8888 - 40 FPS

*/

/*! prepare our thread, etc..  */
Velvet::Renderer::Renderer( SDL_Renderer *sdlRenderer )
	: m_sdlRenderer( sdlRenderer )
{
	if( m_sdlRenderer == nullptr )
   {
       Logger::error( "SDL Renderer failed to initialize!" );
   }

   m_texture = SDL_CreateTexture
        (
            m_sdlRenderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_TARGET,
            VRENDER_WIDTH, VRENDER_HEIGHT
        );

   m_buffer  = std::vector< Pixel >( VRENDER_WIDTH * VRENDER_HEIGHT, { 0, 0, 0, 0 } );
   m_zbuffer = std::vector< float >( VRENDER_WIDTH * VRENDER_HEIGHT, std::numeric_limits< float >::min() );
}

Velvet::Renderer::~Renderer()
{

}

SDL_Renderer* Velvet::Renderer::getInternalRenderer()
{
   return m_sdlRenderer;
}

void Velvet::Renderer::render()
{

    SDL_UpdateTexture
        (
            m_texture,
            NULL,
            &m_buffer[0],
            VRENDER_WIDTH * 4
        );

    SDL_RenderCopy( m_sdlRenderer, m_texture, NULL, NULL );
    SDL_RenderPresent( m_sdlRenderer );

    //memset( m_buffer.data(), 0, sizeof( Pixel ) * VRENDER_WIDTH * VRENDER_HEIGHT );
    memset( m_zbuffer.data(), std::numeric_limits< float >::min(), sizeof( float ) * VRENDER_WIDTH * VRENDER_HEIGHT );
    //std::fill( m_buffer.begin(), m_buffer.end(), 0 );

}

void Velvet::Renderer::drawPixel( int x, int y, Pixel& pixel )
{
    if( x >= VRENDER_WIDTH || x < 0 || y >= VRENDER_HEIGHT || y < 0 )
        return;

    const int offset = ( VRENDER_WIDTH * y ) + x;
    memcpy( m_buffer.data() + offset, &pixel, sizeof( Pixel ) );
}

void Velvet::Renderer::fastCopyScanline( uint32_t y, const void* src, size_t count )
{
   memcpy( m_buffer.data() + ( VRENDER_WIDTH * y ), src, count );
}

void Velvet::Renderer::copyScanline( uint32_t y, const void* src, size_t count )
{
   if( count / sizeof( Velvet::Pixel ) >= VRENDER_WIDTH )
      count = VRENDER_WIDTH * sizeof( Velvet::Pixel );

   if( y >= VRENDER_HEIGHT || y < 0 )
       return;

   memcpy( m_buffer.data() + ( VRENDER_WIDTH * y ), src, count );
}

/*
void Velvet::Renderer::drawLine( Vec2Int p0, Vec2Int p1, Pixel color )
{
   if( ( p0.x > VRENDER_WIDTH && p1.x > VRENDER_WIDTH ) || ( p0.x < 0 && p1.x < 0 ) )
    return;
   if( ( p0.y > VRENDER_HEIGHT && p1.y > VRENDER_HEIGHT ) || ( p0.y < 0 && p1.y < 0 ) )
    return;

   bool yLonger = false;
   int shortLen = p1.y - p0.y;
   int longLen = p1.x - p0.x;

   if( std::abs( shortLen ) > std::abs( longLen ) )
   {
      int swap = shortLen;
      shortLen = longLen;
      longLen = swap;
      yLonger = true;
   }

   int decInc;
   if( longLen == 0 ) 
      decInc = 0;
   else 
      decInc = ( shortLen << 16 ) / longLen;

   if( yLonger )
   {
      if( longLen > 0 )
      {
         longLen += p0.y;
         for( int j = 0x8000 + ( p0.x << 16 ); p0.y <= longLen; ++p0.y )
         {
            drawPixel( j >> 16, p0.y, color );
            j += decInc;
         }
         return;
      }
      longLen += p0.y;
      for( int j = 0x8000 + ( p0.x << 16 ); p0.y >= longLen; --p0.y )
      {
         drawPixel( j >> 16, p0.y, color );
         j -= decInc;
      }
      return;
   }

   if( longLen > 0 )
   {
      longLen += p0.x;
      for( int j = 0x8000 + ( p0.y << 16 ); p0.x <= longLen; ++p0.x )
      {
         drawPixel( p0.x, j >> 16, color );
         j += decInc;
      }
      return;
   }
   longLen += p0.x;

   for( int j = 0x8000 + ( p0.y << 16 ); p0.x >= longLen; --p0.x )
   {
      drawPixel( p0.x, j >> 16, color );
      j -= decInc;
   }
}
*/

