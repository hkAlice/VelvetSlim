#ifndef _RENDERPOOL
#define _RENDERPOOL

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>

#include "RenderThread.h"
#include "../object/Object.h"
#include "ThreadCtx.h"

namespace Velvet
{
    class RenderPool
    {
        public:

        RenderPool();
        ~RenderPool();

        void cycle( std::vector< Velvet::ObjectPtr >& objList );

        private:

        const uint64_t m_maxWaitTimeout = 100000;

        uint64_t m_threadCount{ 8 }; // get hw threadcount and/or pass as arg to ctor

        std::vector< std::thread > m_threads;
        std::vector< RenderThread > m_renderTh;

        std::condition_variable m_frameStepCv;

        std::mutex m_jobMutex;
        std::queue< Velvet::ObjectPtr > m_jobQueue;

        Velvet::ThreadCtx m_threadCtx{ m_jobQueue, m_jobMutex, m_frameStepCv };

        uint64_t m_waitTimeout{ 0 };

        bool m_active;
    };
}


#endif // _RENDERPOOL