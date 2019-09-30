#ifndef _STATE
#define _STATE

#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

#include "../draw/Renderer.h"
#include "../parser/WaveModel.h"
#include "../object/Object.h"
#include "../draw/RenderPool.h"
#include "../resource/ResourceManager.h"

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

        void loadModel( const std::string& path );
        
        Camera camera;

        //todo: ...
        float lightPos;

        private:

        void testVecMov();
        
        void renderObjects( const float frametime );
        void renderModels();

        Velvet::Renderer& m_vRenderer;
        std::vector< Parser::WaveModel > m_models;

        std::vector< ObjectPtr > m_pObjects;

        Velvet::RenderPool m_renderPool;

        uint64_t m_currFrame;

        bool m_renderReady = false;

        Velvet::ResourceManager m_resMgr{ m_vRenderer.getInternalRenderer() };
    };
}


#endif // _STATE