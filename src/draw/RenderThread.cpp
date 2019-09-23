#include "RenderThread.h"

Velvet::RenderThread::RenderThread( Velvet::ThreadCtx& threadCtx )
	: m_threadCtx( threadCtx )
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
            std::unique_lock< std::mutex > lock( m_threadCtx.jobMutex );

            if( m_threadCtx.jobQueue.size() == 0 )
            	m_threadCtx.renderCount++;

            m_threadCtx.frameStepCv.wait(lock, [this]{return !m_threadCtx.jobQueue.empty(); });
            pObj = m_threadCtx.jobQueue.front();
            m_threadCtx.jobQueue.pop();
        }
        pObj->render();

        
    }
}