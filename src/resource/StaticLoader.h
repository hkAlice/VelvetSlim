#ifndef _STATICLOADER
#define _STATICLOADER

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "../logger/Logger.h"

namespace Velvet::Loader
{
	static SDL_Surface* loadImage( const std::string& path )
	{
        //Logger::info( "Loading image " + path );

        SDL_Surface* pImage = IMG_Load( path.c_str() );
        if( !pImage )
        {
            Logger::warn( "Image loading failed" );
            Logger::error( IMG_GetError() );
        }

        return pImage;
	}

    static Mix_Music* loadMusic( const std::string& path )
    {
        Logger::info( "Loading music " + path );

        Mix_Music* pMusic = Mix_LoadMUS( path.c_str() );
        if( !pMusic )
        {
            Logger::warn( "Music loading failed" );
            Logger::error( Mix_GetError() );
        }

        return pMusic;
    }
}

#endif