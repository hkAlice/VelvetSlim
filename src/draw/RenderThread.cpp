#include "RenderThread.h"

Velvet::RenderThread::RenderThread( std::queue< Velvet::ObjectPtr >& jobQueue, std::mutex& jobMutex, std::condition_variable& frameStepCv, bool active )
    : m_jobQueue( jobQueue ), m_jobMutex( jobMutex ), m_frameStepCv( frameStepCv ), m_active( active )
{

}

Velvet::RenderThread::~RenderThread()
{

}

void Velvet::RenderThread::run()
{
    while(true)
    {
        Velvet::ObjectPtr pObj;
        {
            std::unique_lock< std::mutex > lock( m_jobMutex );

            m_frameStepCv.wait(lock, [this]{return !m_jobQueue.empty(); });

            pObj = m_jobQueue.front();
            m_jobQueue.pop();
        }
        pObj->render();
    }
}