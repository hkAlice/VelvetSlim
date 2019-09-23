#ifndef _THREADCTX
#define _THREADCTX

#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

namespace Velvet
{
	class ThreadCtx
	{
	public:
		ThreadCtx( std::queue< Velvet::ObjectPtr >& rJobQueue, std::mutex& rJobMutex, std::condition_variable& rFrameStepCv )
			: jobQueue( rJobQueue ), jobMutex( rJobMutex ), frameStepCv( rFrameStepCv )
			{

			}

        std::condition_variable& frameStepCv;

        std::mutex& jobMutex;
        std::queue< Velvet::ObjectPtr >& jobQueue;

        std::atomic_uint_fast64_t renderCount{ 0 };

        bool active;
	};
}

#endif