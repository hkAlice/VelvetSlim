#include "Velvet.h"
#include <SDL.h>

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>

#include "../libs/termcolor/termcolor.hpp"
#include "logger/Logger.h"
#include "draw/Renderer.h"
#include "data/Model.h"
#include "data/Vec2Int.h"
#include "data/Pixel.h"
#include "data/Vec3.h"
#include "parser/WaveModel.h"
#include "state/State.h"

constexpr static int FRAME_LIMIT = 300;

bool initializeSDLWindow( SDL_Window *&window, uint32_t width, uint32_t height )
{
    Logger::info( "Initializing SDL window (" + std::to_string( width ) + "," + std::to_string( height ) + ")");
    window = SDL_CreateWindow(
        "Velvet",
        SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0
    );

    return ( window != nullptr );
}

bool initializeSDLRenderer( SDL_Window *&window, SDL_Renderer *&renderer )
{
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    return ( renderer != nullptr );
}

int Velvet::VelvetRenderer::init( uint32_t width, uint32_t height )
{
    // TODO: use width/height from arg
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    const std::string windowTitle = "Velvet";

    bool loop = true;
    float alpha = 1.0f;
    uint32_t getticks = 0, frametimedelta = 0, frametimelast = 0, timerSecond = 0;
    float frametime = 0, framespersecond = 0;


    std::cout << termcolor::red << "VelvetSlim Renderer" << termcolor::reset << std::endl;

    SDL_Init( SDL_INIT_VIDEO );

    if( !initializeSDLWindow( window, VRENDER_WIDTH, VRENDER_HEIGHT ) )
    {
        Logger::error( "Failed to initialize SDL Window!" );
        return 1;
    }

    if( !initializeSDLRenderer( window, sdlRenderer ) )
    {
        Logger::error( "Failed to initialize SDL Renderer!" );
        return 1;
    }

  Velvet::Renderer vRenderer = Velvet::Renderer( sdlRenderer );
  Velvet::State vState( vRenderer );

  vState.initWorld();


  while( loop )
  {
    getticks = SDL_GetTicks();
    frametimedelta = getticks - frametimelast;
    frametimelast = getticks;

    frametime = alpha * frametimedelta + ( 1.0 - alpha ) * frametime;

    framespersecond = 1000.0 / frametime;

    if( getticks > timerSecond + 250 )
    {
      SDL_SetWindowTitle( window, ( windowTitle + " [" + std::to_string( ( int )( framespersecond ) ) + "]" ).c_str() );
      timerSecond = getticks;
    }

    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 )
    {
      if( e.type == SDL_QUIT )
      {
        loop = false;
      }
      else if( e.type == SDL_KEYDOWN )
      {
        switch( e.key.keysym.sym )
        {
          case SDLK_w:
          {
            vState.camera.y+=100;
            vRenderer.getClipMask().y += -1;
            vRenderer.getClipMask().w += -1;
            break;
          }
          case SDLK_s:
          {
            vState.camera.y-=100;

            vRenderer.getClipMask().y += 1;
            vRenderer.getClipMask().w += 1;
            break;
          }
          case SDLK_a:
          {
            vState.camera.x+=100;

            vRenderer.getClipMask().x += -1;
            vRenderer.getClipMask().z += -1;
            break;
          }
          case SDLK_d:
          {
            vState.camera.x-=100;

            vRenderer.getClipMask().x += 1;
            vRenderer.getClipMask().z += 1;
            break;
          }
          case SDLK_z:   vState.camera.z+=0.1f; break;
          case SDLK_x: vState.camera.z-=0.1f; break;
          case SDLK_KP_1:   
          {
            vState.camera.distortion += 1.f;
            break;
          }
          case SDLK_KP_3: vState.camera.distortion -= 1.f; break;
          case SDLK_KP_4:   vState.camera.pitch += 0.1f; break;
          case SDLK_KP_6: vState.camera.pitch -= 0.1f; break;
          case SDLK_UP:   
          {
            vRenderer.getClipMask().w += -1;
            break;
          }
          case SDLK_DOWN:   
          {
            vRenderer.getClipMask().y += 1;
            break;
          }
          case SDLK_LEFT:   
          {
            vRenderer.getClipMask().x += 1;

            vState.camera.z += 0.1f;
            break;
          }
          case SDLK_RIGHT:   
          {
            vRenderer.getClipMask().z += 1;

            vState.camera.z -= 0.1f;
            break;
          }
          case SDLK_i:
          {
            vRenderer.setClipInvert( !vRenderer.getClipInvert() );
            break;
          }
          case SDLK_c:
          {
            vRenderer.setClipMode( !vRenderer.getClipMode() );
            vState.lightPos-=0.1f;
            break;
          }
          case SDLK_v:    vState.lightPos+=0.1f; break;
        }
      }
    }

    vState.executeCommandList( frametime );
    vRenderer.render();
  }

  Logger::info( "Terminating" );

  SDL_DestroyWindow( window );
  SDL_Quit();

  return 0;

}

Velvet::StatePtr Velvet::VelvetRenderer::getState() const
{
    return m_pState;
}

Velvet::RendererPtr Velvet::VelvetRenderer::getRenderer() const
{
    return m_vRenderer;
}