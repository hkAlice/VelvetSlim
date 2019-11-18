#ifndef _RENDERER
#define _RENDERER

#include <array>
#include <vector>
#include <SDL.h>

#include "../data/Pixel.h"
#include "../data/Vec2Int.h"
#include "../data/Vec2.h"
#include "../data/Vec3.h"
#include "../data/Vec4Int.h"
#include "../resource/Image.h"

// TODO: Remove all references to SDL renderer, only use window
static constexpr uint32_t VRENDER_WIDTH  = 1280;
static constexpr uint32_t VRENDER_HEIGHT = 720;

namespace Velvet
{
    class Renderer
    {
    public:
        Renderer( SDL_Renderer *sdlRenderer );
        ~Renderer();

        SDL_Renderer* getInternalRenderer();

        void fastDrawPixel( int x, int y, Pixel& pixel );

        void drawPixel( int x, int y, Pixel& pixel );
        // Solid line
        void drawLine( Vec2Int p0, Vec2Int p1, Pixel color );
        // Gradient line
        void drawLineGradient( Vec2Int p0, Vec2Int p1, Pixel color1, Pixel color2 );

        void drawTriangle( Vec3& v0, Vec3& v1, Vec3& v2, Pixel& color );

        void drawBox( Vec2Int p0, Vec2Int p1, Pixel color );
        void drawBox( Vec4Int p0, Pixel color );

        void drawImage( ImagePtr pImage );

        void fastCopyScanline(uint32_t y, const void* src, size_t count);
        void copyScanline(uint32_t y, const void* src, size_t count);

        void render();

        // TODO: private + getset clipmask

        Vec4Int& getClipMask();

        void setClipMode( bool mode );
        void setClipInvert( bool invert );

        bool getClipMode() const;
        bool getClipInvert() const;

    private:

        std::vector< Pixel > m_buffer;

        std::vector< float > m_zbuffer;

        SDL_Renderer* m_sdlRenderer;
        SDL_Texture*  m_texture;

        Vec4Int m_clipMask{ 150, VRENDER_HEIGHT - 100, VRENDER_WIDTH - 600, 50 };
        bool m_isClipping{ true };
        bool m_clipInvert{ true };


    };

    using RendererPtr = std::shared_ptr< Renderer >;

}

#endif // _RENDERER