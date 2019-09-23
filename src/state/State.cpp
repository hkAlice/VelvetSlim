#include "../logger/Logger.h"
#include <cmath>
#include "State.h"
#include "../draw/ColorUtils.h"

Velvet::State::State( Velvet::Renderer& vRenderer )
: m_vRenderer( vRenderer )
{
  camera.x = -1500;
  camera.y = -2050;
  camera.z = 0.35;
  camera.pitch = 0.0;
  camera.yaw = 0.0;
  camera.roll = 0.0;
  camera.distortion = 120.0;

  lightPos = 5.0f;
}

Velvet::State::~State()
{

}

void Velvet::State::initWorld()
{

  for( int x = 0; x < 50; ++x )
  {
    Velvet::ObjectPtr boxTestObj = std::make_shared< Velvet::Object >();
    boxTestObj->setWorkFunction( [&]()
    {

      Pixel boxColor = { 240, 50, 95, 255 };

      for( int i = 0; i < 1000; ++i ) 
      {
        m_vRenderer.drawLine( { 200, 200 }, { i, 200 }, boxColor );
        m_vRenderer.drawLine( { i, 200 }, { 400, 400 }, boxColor );
        m_vRenderer.drawLine( { i, i }, { 200, i }, boxColor );
        m_vRenderer.drawLine( { 200, 400 }, { 200, 200 }, boxColor );
      }

      m_vRenderer.drawLine( { 200, 200 }, { 400, 200 }, boxColor );
      m_vRenderer.drawLine( { 400, 200 }, { 400, 400 }, boxColor );
      m_vRenderer.drawLine( { 400, 400 }, { 200, 400 }, boxColor );
      m_vRenderer.drawLine( { 200, 400 }, { 200, 200 }, boxColor );
    });

    m_pObjects.push_back( boxTestObj );
  }
}

void Velvet::State::loadModel( const std::string& path )
{
// TODO: always assuming WaveModel, but...
// Also this is where our objects would be related
// Function would be createObject, NOT loadModel like it is now

  Velvet::Parser::WaveModel model = Velvet::Parser::WaveModel( path );
  m_models.push_back( model );

  /*
  for( int i = 0; i < 4; ++i )
  {
    //Logger::info( "Render thread [" + std::to_string( i ) + "] has started" );
    m_threads.push_back( std::thread( &Velvet::State::executeFaceQuadrantThreadWork, this, i ) );

    //m_threads[i].detach();
  }*/
}


void Velvet::State::renderModels()
{
  Vec3 trianglePos[3];
  Vec3    wTrigPos[3];
  Velvet::Pixel pixColor = { 255, 255, 255, 255 };

  for( const auto& model : m_models )
  {
    int faceCount = model.getFaceCount();

    for( uint64_t i = 0; i < faceCount; ++i ) 
    { 
      const Velvet::Face& face = model.getFaceAt( i ); 
      for( size_t j = 0; j < 3; ++j )
      {   
        const Velvet::Vec3& v0 = model.getVertAt( face.vertex[j] ); 

        int x0 = v0.x * camera.distortion / 2.f; 
        int y0 = v0.y * camera.distortion / 2.f;
        int z0 = v0.z * camera.distortion / 2.f;

        x0 = ( x0 - camera.x ) * camera.z;
        y0 = ( y0 - camera.y ) * camera.z;
        z0 = ( z0 - camera.z ) * camera.z;

// rotation attempt #1

        auto tmpV = x0;
/*
x0 = x0 * std::cos( camera.pitch ) - y0 * std::sin( camera.pitch );
y0 = tmpV * std::cos( camera.pitch ) + y0 * std::cos( camera.pitch );*/

        trianglePos[j] = { (float)x0, (float)y0, (float)z0 };
        wTrigPos[j] = v0;

//m_vRenderer.drawLine( { x0, y0 }, { x1, y1 }, pixColor );
      }

      Vec3 shLightTrig = wTrigPos[2] - wTrigPos[0];
      Vec3 shLightRhs = wTrigPos[1] - wTrigPos[0];
      shLightTrig = shLightTrig ^ shLightRhs;

      float intensity = shLightTrig.length() * lightPos;
      if( intensity > 0.f ) 
      {
        intensity = intensity > 1.0 ? 1.0 : intensity;
        pixColor.r = intensity * 255;
        pixColor.g = intensity * 255;
        pixColor.b = intensity * 255;

        m_vRenderer.drawTriangle( trianglePos[0], trianglePos[1], trianglePos[2], pixColor );
      } 
    }
  }
}

void Velvet::State::renderObjects( const float frametime )
{
  /*
  for( auto& obj : m_pObjects )
  {
    m_threads.push_back( std::thread( &Velvet::Object::render, obj ) );
  }

  for( auto& thread : m_threads )
  {
    if( thread.joinable() )
      thread.join();
  }*/

  for( auto& pObj : m_pObjects )
    pObj->render();
}

void Velvet::State::executeCommandList( float frametime )
{
  m_currFrame++;

  //renderObjects( frametime );
  //renderModels();

  m_renderPool.cycle( m_pObjects );
}