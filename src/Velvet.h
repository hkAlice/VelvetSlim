#ifndef _VELVET
#define _VELVET

#include "draw/Renderer.h"
#include "state/State.h"

namespace Velvet
{

class VelvetRenderer
{

public:
    VelvetRenderer()
    {

    }

    ~VelvetRenderer() = default;
    int init( uint32_t width, uint32_t height );

    StatePtr getState() const;
    RendererPtr getRenderer() const;

protected:

    StatePtr m_pState;
    RendererPtr m_vRenderer;

};

}

#endif