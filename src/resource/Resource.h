#ifndef _RESOURCE
#define _RESOURCE

#include <memory>

namespace Velvet
{
    class IResource
    {
    public:
        IResource(){};
        virtual ~IResource() = default;

    private:

    };
}

using ResourcePtr = std::shared_ptr< Velvet::IResource >;

#endif // _RESOURCE