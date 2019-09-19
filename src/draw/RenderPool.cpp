#include "RenderPool.h"

#include "../logger/Logger.h"

Velvet::RenderPool::RenderPool()
	: m_active( true )
{
	for( int i = 0; i < m_threadCount; ++i )
	{
		auto renderTh = Velvet::RenderThread( m_jobQueue, m_jobMutex, m_frameStepCv, m_active );
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
	m_frameStepCv.notify_all();
}

void Velvet::RenderPool::cycle( std::vector< Velvet::ObjectPtr >& objList )
{
	for( const auto& pObj : objList )
	{
		m_jobQueue.push( pObj );
	}

	m_frameStepCv.notify_all();
	
}