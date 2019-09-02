#ifndef _RENDERER
#define _RENDERER

#include <array>
#include <vector>
#include <SDL.h>

#include "Pixel.h"
#include "../data/Vec2Int.h"
#include "../data/Vec2.h"
#include "../data/Vec3.h"

// TODO: Remove all references to SDL renderer, only use window
static constexpr int VRENDER_WIDTH  = 1650;
static constexpr int VRENDER_HEIGHT = 1050;

namespace Velvet
{
    class Renderer
    {
    public:
        Renderer( SDL_Renderer *sdlRenderer );
        ~Renderer(){};

        void drawPixel( int x, int y, Pixel& pixel );
        // Solid line
        void drawLine( Vec2Int p0, Vec2Int p1, Pixel color );
        // Gradient line
        void drawLine( Vec2Int p0, Vec2Int p1, Pixel color1, Pixel color2 );

        void drawTriangle( Vec3& v0, Vec3& v1, Vec3& v2, Pixel& color );

        void render();

    private:

        std::vector< Pixel > m_buffer;

        std::vector< float > m_zbuffer;

        SDL_Renderer* m_sdlRenderer;
        SDL_Texture*  m_texture;

    };

}

#endif // _RENDERER