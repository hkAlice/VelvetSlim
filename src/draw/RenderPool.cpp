#include "RenderPool.h"

#include "../logger/Logger.h"

Velvet::RenderPool::RenderPool()
	: m_active( true )
{
    Logger::info( "RenderPool threads: [" + std::to_string( m_threadCount ) + "]" );

	for( int i = 0; i < m_threadCount; ++i )
	{
		auto renderTh = Velvet::RenderThread( m_threadCtx );
		m_renderTh.push_back( renderTh );
		m_threads.push_back( std::thread( &Velvet::RenderThread::run, renderTh ) );
	}

	for( auto& thread : m_threads )
	{
		//thread.join();
	}

	m_frameStepCv.notify_all();
}

Velvet::RenderPool::~RenderPool()
{
	Logger::info("Shutting down render pool");
	m_active = false;

   for( auto& thread : m_threads )
   {
      thread.join();
   }
	m_frameStepCv.notify_all();
}

void Velvet::RenderPool::cycle( std::vector< Velvet::ObjectPtr >& objList )
{
	for( const auto& pObj : objList )
	{
		m_jobQueue.push( pObj );
	}

	m_frameStepCv.notify_all();

	while( m_threadCtx.renderCount < m_threadCount )
	{
      m_frameStepCv.notify_all();
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		//Logger::info(std::to_string(m_threadCtx.renderCount));
		//m_threadCtx.renderCount = 0;
	}

	{
      std::unique_lock< std::mutex > lock( m_threadCtx.jobMutex );
		m_threadCtx.renderCount = 0;
    }

}