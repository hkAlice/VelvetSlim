#ifndef _STATE
#define _STATE

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

#include "../draw/Renderer.h"
#include "../parser/WaveModel.h"
#include "../object/Object.h"

namespace Velvet
{
    // todo: vec3int..
    struct Camera
    {
        float x;
        float y;
        float z;
        float pitch;
        float yaw;
        float roll;
        float distortion;
    };

    class State
    {
        public:

        State( Velvet::Renderer& vRenderer );
        ~State();

        void initWorld();

        // TODO: add object, remove object

        void executeCommandList( float frametime );

        void executeFaceQuadrantThreadWork( int threadIndex );

        void loadModel( const std::string& path );

        // TODO: Object class and system
        // Every entity that has a position in the render scene
        // is an object with something associated to it 
        // eg a model, etc.

        // Maybe object should hold a function of draw actions to process?
        // ex. []( RendererPtr vR, Stack& st ) { vR->drawLine(300,300, ...) };
        
        Camera camera;

        //todo: ...
        float lightPos;

        private:

        void renderObjects( const float frametime );
        void renderModels();

        Velvet::Renderer& m_vRenderer;
        std::vector< Parser::WaveModel > m_models;

        std::vector< Object > m_objects;

        std::vector< std::thread > m_threads;

        std::mutex m_mtx;

        std::condition_variable m_renderStep;
        std::condition_variable m_renderDone;

        uint64_t m_currFrame;

        bool m_renderReady = false;
    };
}


#endif // _STATE