#ifndef _RENDERPOOL
#define _RENDERPOOL

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>

#include "RenderThread.h"
#include "../object/Object.h"

namespace Velvet
{
    class RenderPool
    {
        public:

        RenderPool();
        ~RenderPool();

        void cycle( std::vector< Velvet::ObjectPtr >& objList );

        private:

        uint64_t m_threadCount { 1 }; // get hw threadcount and/or pass as arg to ctor

        void renderObjects( const float frametime );
        void renderModels();

        std::vector< std::thread > m_threads;
        std::vector< RenderThread > m_renderTh;

        std::condition_variable m_frameStepCv;

        std::mutex m_jobMutex;
        std::queue< Velvet::ObjectPtr > m_jobQueue;

        bool m_active;
    };
}


#endif // _RENDERPOOL