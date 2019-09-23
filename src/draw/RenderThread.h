#ifndef _RENDERTHREAD
#define _RENDERTHREAD

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>

#include "../draw/Renderer.h"
#include "../object/Object.h"
#include "ThreadCtx.h"

namespace Velvet
{
    class RenderThread
    {
        public:

        RenderThread( Velvet::ThreadCtx& threadCtx );
        ~RenderThread();

        void run();

        private:

        Velvet::ThreadCtx& m_threadCtx;

        bool m_active;

    };
}


#endif // _RENDERTHREAD