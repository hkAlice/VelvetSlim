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

namespace Velvet
{
    class RenderThread
    {
        public:

        RenderThread( std::queue< Velvet::ObjectPtr >& jobQueue, std::mutex& jobMutex, std::condition_variable& frameStepCv, bool active );
        ~RenderThread();

        void run();

        private:

        std::condition_variable& m_frameStepCv;

        std::mutex& m_jobMutex;
        std::queue< Velvet::ObjectPtr >& m_jobQueue;

        bool m_active;

    };
}


#endif // _RENDERTHREAD