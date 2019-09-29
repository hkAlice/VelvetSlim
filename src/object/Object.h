#ifndef _OBJECT
#define _OBJECT

#include <vector>
#include <functional>
#include <memory>
#include "../resource/Resource.h"

namespace Velvet
{
    class Object
    {
    public:

        Object();
        ~Object() = default;

        // TODO: Object class and system
        // Every entity that has a position in the render scene
        // is an object with something associated to it 
        // eg a model, etc.

        // Maybe object should hold a function of draw actions to process?
        // ex. []( RendererPtr vR, Stack& st ) { vR->drawLine(300,300, ...) };

        void setWorkFunction( std::function<void()> workFunc );

        const void render();

    private:

        std::function<void()> m_workFunc;

    };

    using ObjectPtr = std::shared_ptr< Object >;
}


#endif // _STATE