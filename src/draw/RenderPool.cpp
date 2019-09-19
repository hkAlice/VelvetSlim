#include "RenderPool.h"

#include "../logger/Logger.h"

Velvet::RenderPool::RenderPool()
: m_active( true )
{
    Logger::info( "RenderPool threads: [" + std::to_string( m_threadCount ) + "]" );

    for( int i = 0; i < m_threadCount; ++i )
    {
        auto renderTh = Velvet::RenderThread( m_jobQueue, m_jobMutex, m_frameStepCv, m_active );
        m_renderTh.push_back( renderTh );
        m_threads.push_back( std::thread( &Velvet::RenderThread::run, renderTh ) );
    }

    for( auto& thread : m_threads )
    {
    }

    m_frameStepCv.notify_all();
}

Velvet::RenderPool::~RenderPool()
{
    Logger::info( "Shutting down render pool" );

    m_active = false;
    m_frameStepCv.notify_all();

    for( auto& thread : m_threads )
    {
        thread.join();
    }
}

void Velvet::RenderPool::cycle( std::vector< Velvet::ObjectPtr >& objList )
{
    {
       

        std::unique_lock< std::mutex > lg( m_jobMutex );

        m_frameStepCv.wait(lg, [this]{ return m_jobQueue.size() <= 1; });

        for( const auto& pObj : objList )
        {
            m_jobQueue.push( pObj );
            
        }

        m_frameStepCv.notify_all();
    }

    for( auto& thread : m_threads )
    {
        //thread.join();
    }
    
    std::unique_lock< std::mutex > lock( m_jobMutex );

    //m_frameStepCv.wait(lock, [this]{ return m_jobQueue.empty(); });

    Logger::warn("?");
}