#include "../logger/Logger.h"
#include <cmath>
#include <math.h>
#include "State.h"
#include "../draw/ColorUtils.h"

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

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
/*
  for( int x = 0; x < 15; ++x )
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
  }*/
  static float controlTime = 0.f;

  Velvet::ObjectPtr fractal = std::make_shared< Velvet::Object >();
  fractal->setWorkFunction( [&]()
  {
    // Lévy C curve
    // adapted from: https://en.wikipedia.org/wiki/L%C3%A9vy_C_curve

    std::function<void( double, double, double, double, int )> c_curve;
    c_curve = [&]( double x, double y, double len, double alpha_angle, int iteration_n  )
    {
       double fx = x;
       double fy = y;
       double length = len;
       double alpha = alpha_angle;
       int it_n = iteration_n;
       if( it_n > 0 ) 
       {
          length = ( length / std::sqrt( 2 ) );
          c_curve( fx, fy, length, ( alpha + 45 ), ( it_n - 1 ) );
          fx = ( fx + ( length * std::cos( degToRad( alpha + 45 ) ) ) );
          fy = ( fy + ( length * std::sin( degToRad( alpha + 45 ) ) ) );
          c_curve( fx, fy, length, ( alpha - 45 ), ( it_n - 1 ) );
       }
       else 
       {
          Pixel c1 = { 128, 0, 255, 255 };//Velvet::ColorUtils::randomPixelColor();
          Vec2Int p1 = { ( int )fx, ( int )fy };
          Vec2Int p2 = { ( int )( fx + ( length * std::cos( degToRad( alpha ) ) ) ), ( int )( fy + ( length * std::sin( degToRad( alpha ) ) ) ) };
          m_vRenderer.drawLine( p1, p2, c1 );
       }
    };

    c_curve( 400, 280, 720, 0, (int)controlTime % 17 );

    controlTime += 0.15;

    if( controlTime > 400 )
       controlTime = 0;
  });

  //m_pObjects.push_back( fractal );

  auto imgTest = std::make_shared< Velvet::Object >();
  imgTest->setWorkFunction( [&]()
  {
      Logger::info( "awaa" );
      auto pImg = m_resMgr.get< Velvet::Image >( "yo000205.bmp" );
	   m_vRenderer.drawImage( pImg );
  });

  m_pObjects.push_back( imgTest );
  
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
  for( auto& pObj : m_pObjects )
    pObj->render();
}

void Velvet::State::executeCommandList( float frametime )
{
  m_currFrame++;

  renderObjects( frametime );
  //renderModels();

  //m_renderPool.cycle( m_pObjects );
}