#ifndef _MUSICRESOURCE
#define _MUSICRESOURCE

#include "Resource.h"
#include <SDL_mixer.h>

namespace Velvet
{
    class Music : public IResource
    {
    public:
        Music( Mix_Music* music )
        : m_music( music ) 
        {
            Mix_PlayMusic( music, -1 );
        };
        virtual ~Music()
        {
		      Mix_FreeMusic( m_music );
        };

        void play()
        {
            if( !isPlaying() )
                Mix_ResumeMusic();
        }

        void pause()
        {
            Mix_PauseMusic();
        }

        void stop()
        {
            Mix_HaltMusic();
        }

        bool isPlaying() const
        {
            return Mix_PlayingMusic() == 1;
        }

        Mix_Music* getMusic() const
        {
            return m_music;
        };

    private:
        Mix_Music* m_music;
    };

}

using MusicResourcePtr = std::shared_ptr< Velvet::Music >;
using MusicPtr = std::shared_ptr< Velvet::Music >;

#endif // _MUSICRESOURCE