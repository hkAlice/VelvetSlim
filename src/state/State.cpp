﻿#include "../logger/Logger.h"
#include <cmath>
#include <math.h>
#include <iomanip>
#include "State.h"
#include "../draw/ColorUtils.h"
#include "../experimental/fastVU.h"

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

void Velvet::State::testVecMov()
{
  static std::string extension = ".png";

  auto imgTest = std::make_shared< Velvet::Object >();
  //static uint32_t idx = 1;
  static uint32_t idx = 0;

  static std::vector< ResourcePtr > imgRes;

  imgTest->setWorkFunction( [&]()
  {
      // todo: each obj has its stored res pts
      
      if( idx % 10 == 0 )
      {
         for( const auto& pRes : imgRes )
         {
            m_resMgr.remove( pRes->getName() );
         }

         imgRes.clear();
      }

      std::ostringstream ss;
      ss << std::setw( 3 ) << std::setfill( '0' ) << idx;
      std::string str = ss.str();

      //auto pImg = m_resMgr.get< Velvet::Image >( "outmm32/" + str + extension );
      auto pImg = m_resMgr.get< Velvet::Image >( "outmm32/" + str + extension );
      m_vRenderer.drawImage( pImg );

      imgRes.push_back( pImg );

      idx++;
  });

  m_pObjects.push_back( imgTest );
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
  static std::string extension = ".png";

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
          Pixel c1 = { 255, 128, 0, 255 };//Velvet::ColorUtils::randomPixelColor();
          Vec2Int p1 = { ( int )fx, ( int )fy };
          Vec2Int p2 = { ( int )( fx + ( length * std::cos( degToRad( alpha ) ) ) ), ( int )( fy + ( length * std::sin( degToRad( alpha ) ) ) ) };
          m_vRenderer.drawLine( p1, p2, c1 );
       }
    };

    c_curve( 400, 280, 720, 0, (int)controlTime % 17 );

    controlTime += 0.10;

    if( controlTime > 50 )
       controlTime = 0;

  });

  auto imgTest = std::make_shared< Velvet::Object >();
  static uint32_t idx = 1;
  //static uint32_t idx = 7200;

  static std::vector< ResourcePtr > imgRes;

  imgTest->setWorkFunction( [&]()
  {
      // todo: each obj has its stored res pts
      
      if( idx % 10 == 0 )
      {
         for( const auto& pRes : imgRes )
         {
            m_resMgr.remove( pRes->getName() );
         }

         imgRes.clear();
      }

      std::ostringstream ss;
      ss << std::setw( 3 ) << std::setfill( '0' ) << idx;
      std::string str = ss.str();

      auto pImg = m_resMgr.get< Velvet::Image >( "outmm32/" + str + extension );
      //auto pImg = m_resMgr.get< Velvet::Image >( "outbd32/" + str + extension );
	    m_vRenderer.drawImage( pImg );

      imgRes.push_back( pImg );

      idx++;
  });

  auto staticEffect = std::make_shared< Velvet::Object >();
  staticEffect->setWorkFunction( [&]()
  {
    uint32_t arrRand[1280];
    Pixel randWhiteBlack;

    for ( int y = 0; y < 720; ++y )
    {
      for ( int x = 0; x < 1280; ++x )
      {
        
        if( fastrand() % 2 == 0 )
          arrRand[x] = -1;
        else
          arrRand[x] = 0;
      }

      m_vRenderer.copyScanline( y, arrRand, 1280 * 4 );
   }
  });

  //m_pObjects.push_back( imgTest );
  m_pObjects.push_back( fractal );

  //m_pObjects.push_back( staticEffect );

  //testVecMov();

  //loadModel("4.obj");
  
}

void Velvet::State::loadModel( const std::string& path )
{
// TODO: always assuming WaveModel, but...
// Also this is where our objects would be related
// Function would be createObject, NOT loadModel like it is now

  Velvet::Parser::WaveModel model = Velvet::Parser::WaveModel( path );
  m_models.push_back( model );
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
  renderModels();

  //m_renderPool.cycle( m_pObjects );
}