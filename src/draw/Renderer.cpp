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
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            VRENDER_WIDTH, VRENDER_HEIGHT
        );

   m_buffer  = std::vector< Pixel >( VRENDER_WIDTH * VRENDER_HEIGHT, { 0, 0, 0, 0 } );
   m_zbuffer = std::vector< float >( VRENDER_WIDTH * VRENDER_HEIGHT, std::numeric_limits< float >::min() );
}

Velvet::Renderer::~Renderer()
{

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

    memset( m_buffer.data(), 0, sizeof( Pixel ) * VRENDER_WIDTH * VRENDER_HEIGHT );
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

void Velvet::Renderer::drawLine( Vec2Int p0, Vec2Int p1, Pixel color )
{
    if( ( p0.x > VRENDER_WIDTH && p1.x > VRENDER_WIDTH ) || ( p0.x < 0 && p1.x < 0 ) )
      return;
    if( ( p0.y > VRENDER_HEIGHT && p1.y > VRENDER_HEIGHT ) || ( p0.y < 0 && p1.y < 0 ) )
      return;

    const bool steep = ( std::abs( p1.y - p0.y ) > std::abs( p1.x - p0.x ) );
    if( steep )
    {
        std::swap( p0.x, p0.y );
        std::swap( p1.x, p1.y );
    }
    
    if( p0.x > p1.x )
    {
        std::swap( p0.x, p1.x );
        std::swap( p0.y, p1.y );
    }
    
    const int dx = p1.x - p0.x;
    const int dy = std::abs( p1.y - p0.y );
    
    int error = dx / 2;
    const int ystep = ( p0.y < p1.y ) ? 1 : -1;
    int y = p0.y;
    
    const int maxX = p1.x;

    for( int x = p0.x; x < maxX; ++x )
    {
        if( steep )
        {
            drawPixel( y, x, color );
        }
        else
        {
            drawPixel( x, y, color );
        }
    
        error -= dy;
        if( error < 0 )
        {
            y += ystep;
            error += dx;
        }
    }
}

// TODO: Maybe use EFLA+Color Mixing via divergence
void Velvet::Renderer::drawLineGradient( Vec2Int p0, Vec2Int p1, Pixel color1, Pixel color2 )
{
    const bool steep = ( std::abs( p1.y - p0.y ) > std::abs( p1.x - p0.x ) );
    if( steep )
    {
        std::swap( p0.x, p0.y );
        std::swap( p1.x, p1.y );
    }
    
    if( p0.x > p1.x )
    {
        std::swap( p0.x, p1.x );
        std::swap( p0.y, p1.y );
    }
    
    const int dx = p1.x - p0.x;
    const int dy = std::abs( p1.y - p0.y );
    
    int error = dx / 2;
    const int ystep = ( p0.y < p1.y ) ? 1 : -1;
    int y = p0.y;
    
    const int maxX = p1.x;

    for( int x = p0.x; x < maxX; ++x )
    {
        float divergence = float( x ) / float( maxX );

        Pixel color;
        color.r = color1.r * ( 1.0f - divergence ) + color2.r * divergence;
        color.g = color1.g * ( 1.0f - divergence ) + color2.g * divergence;
        color.b = color1.b * ( 1.0f - divergence ) + color2.b * divergence;
        color.a = color1.a * ( 1.0f - divergence ) + color2.a * divergence;

        if( steep )
        {
            drawPixel( y, x, color );
        }
        else
        {
            drawPixel( x, y, color );
        }
    
        error -= dy;
        if( error < 0 )
        {
            y += ystep;
            error += dx;
        }
    }
}

void barycentric( Velvet::Vec3& v0, Velvet::Vec3& v1, Velvet::Vec3& v2, Velvet::Vec3& P, Velvet::Vec3& result )
{ 
    Vec3 u = { v2.x - v0.x, v1.x - v0.x, v0.x - P.x };
    Vec3 yU = { v2.y - v0.y, v1.y - v0.y, v0.y - P.y };
    u = u.cross( yU );

    if( std::abs( u.z ) < 1 )
    {
       result.x = -1.0;
       result.y = 1.0;
       result.z = 1.0;
       return;
    }

    result.x = 1.f - ( u.x + u.y ) / u.z;
    result.y = u.y / u.z;
    result.z = u.x / u.z;
} 

float calcDot( Velvet::Vec3& v0, Velvet::Vec3& v1 )
{
   return v0.x * v1.x +
          v0.y * v1.y +
          v0.z * v1.z;
}

/*
Velvet::Vec3 barycentric( Velvet::Vec3& v0, Velvet::Vec3& v1, Velvet::Vec3& v2, Velvet::Vec3& P )
{
   Vec3 bv0 = v1 - v0, bv1 = v2 - v0, bv2 = P - v0;
   float d00 = calcDot( v0, v0 );
   float d01 = calcDot( v0, v1 );
   float d11 = calcDot( v1, v1 );
   float d20 = calcDot( v2, v0 );
   float d21 = calcDot( v2, v1 );
   float denom = d00 * d11 - d01 * d01;

   float v = ( d11 * d20 - d01 * d21 ) / denom;
   float w = ( d00 * d21 - d01 * d20 ) / denom;
   float u = 1.0f - v - w;

   return { u, v, w };
}*/

/*
Velvet::Vec3 barycentric( Velvet::Vec3 v0, Velvet::Vec3 v1, Velvet::Vec3 v2, Velvet::Vec3 P )
{ 
    Vec3 s1, s2;
    
    s2.x = v2.y-v0.y;
    s2.y = v1.y-v0.y;
    s2.z = v0.y-P.y;

    s1.x = v2.x-v0.x;
    s1.y = v1.x-v0.x;
    s1.z = v0.x-P.x;

    Vec3 u = s1.cross( s2 );

    if( std::abs(u.z) > 1e-2 ) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return { 1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z };

    return { -1,1,1 };
} */
/*
void Velvet::Renderer::drawTriangle( Vec2Int v0, Vec2Int v1, Vec2Int v2, Pixel color )
{
    
    drawLine( v0, v1, color );
    drawLine( v1, v2, color );
    drawLine( v2, v0, color );
}

*/

/*
void Velvet::Renderer::drawTriangle( Vec2Int& v0, Vec2Int& v1, Vec2Int& v2, Pixel& color )
{
  
    drawLine( v0, v1, color );
    drawLine( v1, v2, color );
    drawLine( v2, v0, color );

    if( v0.y > v1.y ) Vec2Int::swap( v0, v1 );
    if( v0.y > v2.y ) Vec2Int::swap( v0, v2 );
    if( v1.y > v2.y ) Vec2Int::swap( v1, v2 );

    auto total_height = v2.y - v0.y; 
    for( int i = 0; i < total_height; ++i )
    { 
        bool second_half = i>v1.y-v0.y || v1.y==v0.y;
        int segment_height = second_half ? v2.y-v1.y : v1.y-v0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? v1.y-v0.y : 0))/segment_height;
        Vec2Int uA = (v2 - v0) * alpha;
        Vec2Int uB1 = ( v2 - v1 ) * beta;
        Vec2Int uB2 = ( v1 - v0 ) * beta;
        Vec2Int A =               v0 + uA;
        Vec2Int B = second_half ? v1 + uB1 : v0 + uB2;

        if (A.x > B.x) Vec2Int::swap( A, B ); 

        //if( A.x > VRENDER_WIDTH || A.x < 0 ||  )

        auto bV = barycentric( v0, v1, v2, { A.x, B.x } );

        if( bV.x < 0 || bV.y < 0 || bV.z < 0 ) continue;
        Logger::info( bV.x );
        Logger::info( bV.y );
        Logger::info( bV.z );
        Logger::info( "====" );

        Vec2Int rX = { A.x, v0.y + i };
        Vec2Int rY = { B.x, v0.y + i };
        //drawLine( { A.x, v0.y + i }, { B.x, v0.y + i }, color );
        drawLine( rX, rY, color );

    }
}
*/

void Velvet::Renderer::drawTriangle( Vec3& v0, Vec3& v1, Vec3& v2, Pixel& color )
{
    Vec2 bboxmin{ std::numeric_limits< float >::max(),  std::numeric_limits< float >::max() };
    Vec2 bboxmax{ std::numeric_limits< float >::min(), std::numeric_limits< float >::min() };
    Vec2 clamp{ VRENDER_WIDTH - 1, VRENDER_HEIGHT - 1 };

    bboxmin.x = std::max(0.f,      std::min(bboxmin.x, v0.x));
    bboxmax.x = std::min(clamp.x,  std::max(bboxmax.x, v0.x));
    bboxmin.y = std::max(0.f,      std::min(bboxmin.y, v0.y));
    bboxmax.y = std::min(clamp.y,  std::max(bboxmax.y, v0.y));
    bboxmin.x = std::max(0.f,      std::min(bboxmin.x, v1.x));
    bboxmax.x = std::min(clamp.x,  std::max(bboxmax.x, v1.x));
    bboxmin.y = std::max(0.f,      std::min(bboxmin.y, v1.y));
    bboxmax.y = std::min(clamp.y,  std::max(bboxmax.y, v1.y));
    bboxmin.x = std::max(0.f,      std::min(bboxmin.x, v2.x));
    bboxmax.x = std::min(clamp.x,  std::max(bboxmax.x, v2.x));
    bboxmin.y = std::max(0.f,      std::min(bboxmin.y, v2.y));
    bboxmax.y = std::min(clamp.y,  std::max(bboxmax.y, v2.y));

    Vec3 P{ 0.f, 0.f, 0.f };
    Vec3 bcResult;

    for( P.x = bboxmin.x; P.x <= bboxmax.x; ++P.x ) 
    {
        for( P.y = bboxmin.y; P.y <= bboxmax.y; ++P.y ) 
        {
            barycentric( v0, v1, v2, P, bcResult );
            if( bcResult.x < 0 || bcResult.y < 0 || bcResult.z < 0 ) continue;
            P.z = 0.f;
            P.z += v0.z * bcResult.x;
            P.z += v1.z * bcResult.y;
            P.z += v2.z * bcResult.z;
            
            auto& zBufferVal = m_zbuffer[( P.x + P.y * VRENDER_WIDTH )];

            if( zBufferVal < P.z )
            {
                zBufferVal = P.z;
                drawPixel( P.x, P.y, color );
            }
        }
    }

}