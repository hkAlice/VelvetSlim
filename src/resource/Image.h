#ifndef _SPRITERESOURCE
#define _SPRITERESOURCE

#include "Resource.h"
#include "../logger/Logger.h"

namespace Velvet
{
    class Image : public IResource
    {
    public:
       Image( const std::string& resName, SDL_Surface* surface )
          : IResource( resName ), m_surface(surface)
        {
            m_rect.x = 0;
            m_rect.y = 0;
            //SDL_QueryTexture( surface, NULL, NULL, &m_rect.w, &m_rect.h );
		      //SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );

            m_texWidth  = m_rect.w;
            m_texHeight = m_rect.h;
        };
        virtual ~Image()
        {
		      SDL_FreeSurface( m_surface );
        };

        SDL_Surface* getSurface() const
        {
            return m_surface;
        };

        // todo: scale (1.25x etc)

        void x( int xVal )
        {
            m_rect.x = xVal;
        }

        void y( int yVal )
        {
            m_rect.y = yVal;
        }

        void width( int wVal )
        {
            m_rect.w = wVal;
        }

        void height( int hVal )
        {
            m_rect.h = hVal;
        }

        void scale( float scaleVal )
        {
            m_rect.w = m_texWidth * scaleVal;
            m_rect.h = m_texHeight * scaleVal;
        }

		void alpha( float alphaValF )
		{
			if( alphaValF > 1.f )
				alphaValF = 1.f;
			if( alphaValF < 0.f )
				alphaValF = 0.f;

			m_alpha = alphaValF * 255;
			//SDL_SetTextureAlphaMod( m_texture, m_alpha );
		}

		void alpha( uint8_t alphaVal8 )
		{
			m_alpha = alphaVal8;
			//SDL_SetTextureAlphaMod( m_texture, m_alpha );
		}

      SDL_Rect& getRect()
      {
         return m_rect;
      }

    private:
        SDL_Surface* m_surface;
        SDL_Rect m_rect;

        uint32_t m_texWidth;
        uint32_t m_texHeight;

	     uint8_t m_alpha;
    };

}

using ImageResourcePtr = std::shared_ptr< Velvet::Image >;
using ImagePtr = std::shared_ptr< Velvet::Image >;

#endif // _SPRITERESOURCE