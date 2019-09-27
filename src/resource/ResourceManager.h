#ifndef _RESOURCEMANAGER
#define _RESOURCEMANAGER

#include <array>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <SDL.h>

#include "../logger/Logger.h"
#include "Resource.h"
#include "Image.h"
#include "Music.h"

#include "StaticLoader.h"
#include "../forward.h"

// todo: template on resource type

namespace Velvet
{
    class ResourceManager
    {
    public:
        ResourceManager( SDL_Renderer* sdlRenderer )
        : m_sdlRenderer( sdlRenderer ) 
        {

        };

        ~ResourceManager(){};

        template< class T >
        std::shared_ptr< T > get( const std::string& handle );

        bool add( const std::string& handle, ResourcePtr resource )
        {
            auto it = m_resourceMap.find( handle );
            if( it != m_resourceMap.end() ) 
            {
                Logger::warn( "Add already existing resource with handle " + handle  );
                return false;
            }

            m_resourceMap.emplace( handle, resource );
            return true;
        }

    private:

        template< class T >
        std::shared_ptr< T > getFromMap( const std::string& handle )
        {
            auto it = m_resourceMap.find( handle );
            if( it != m_resourceMap.end() ) 
            {
                auto pRes = std::dynamic_pointer_cast< T, IResource >( it->second );
                return pRes;
            }

            return nullptr;
        }

        std::unordered_map< std::string, ResourcePtr > m_resourceMap;
        SDL_Renderer* m_sdlRenderer;
    };

}

template< class T >
std::shared_ptr< T > Velvet::ResourceManager::get( const std::string& handle )
{
    return Velvet::ResourceManager::getFromMap< T >( handle );
}

template <> inline 
std::shared_ptr< Velvet::Image > Velvet::ResourceManager::get< Velvet::Image >( const std::string& handle )
{
    if( auto pRes = Velvet::ResourceManager::getFromMap< Image >( handle ); pRes )
        return pRes;

    auto pImage = Velvet::Loader::loadImage( "res/img/" + handle );

    auto pImageRes = std::make_shared< Velvet::Image >( pImage );
    
    // todo: Velvet::ResourceManager::add 
    m_resourceMap.emplace( handle, std::dynamic_pointer_cast< IResource, Image >( pImageRes ) );
    return pImageRes;
}

template <> inline
std::shared_ptr< Velvet::Music > Velvet::ResourceManager::get< Velvet::Music >( const std::string& handle )
{
    if( auto pRes = Velvet::ResourceManager::getFromMap< Music >( handle ); pRes )
        return pRes;

    auto pMusic = Velvet::Loader::loadMusic( "res/mus/" + handle );

    auto pMusicRes = std::make_shared< Velvet::Music >( pMusic );

    // todo: Velvet::ResourceManager::add 
    m_resourceMap.emplace( handle, std::dynamic_pointer_cast< IResource, Music >( pMusicRes ) );
    return pMusicRes;
}


#endif // _RESOURCEMANAGER