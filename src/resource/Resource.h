#ifndef _RESOURCE
#define _RESOURCE

#include <memory>

namespace Velvet
{
    class IResource
    {
    public:
        IResource( const std::string& resName )
           : m_resName( resName ) {};
        virtual ~IResource() = default;

        const std::string& getName()
        {
           return m_resName;
        }

    protected:
        const std::string m_resName;

    private:
       
    };
}

using ResourcePtr = std::shared_ptr< Velvet::IResource >;

#endif // _RESOURCE