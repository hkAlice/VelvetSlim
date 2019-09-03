#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "WaveModel.h"
#include "../data/Model.h"
#include "../data/Vec2.h"
#include "../data/Vec3.h"
#include "../logger/Logger.h"

Velvet::Parser::WaveModel::WaveModel( const std::string& path )
{
   Logger::info( "Loading WaveModel " + path );

   std::ifstream ifs( path.c_str(), std::ifstream::in );
   std::string line, key;
   while( ifs.good() && !ifs.eof() && std::getline( ifs, line ) ) {
      key = "";
      std::stringstream stringstream( line );
      stringstream >> key >> std::ws;

      if( key == "v" ) { // vertex
         Vertex v; float x;
         stringstream >> x >> std::ws;
         v.x = x + 15;
         stringstream >> x >> std::ws;
         v.y = x * -1;
         stringstream >> x >> std::ws;
         v.z = x + 15;

         m_verts.push_back( v );
      }/*
      else if( key == "vp" ) { // parameter
         Vertex v; float x;
         while( !stringstream.eof() ) {
            stringstream >> x >> std::ws;
            v.v.push_back( x );
         }
         parameters.push_back( v );
      }
      else if( key == "vt" ) { // texture coordinate
         Vertex v; float x;
         while( !stringstream.eof() ) {
            stringstream >> x >> std::ws;
            v.v.push_back( x );
         }
         texcoords.push_back( v );
      }
      else if( key == "vn" ) { // normal
         vertex v; float x;
         while( !stringstream.eof() ) {
            stringstream >> x >> std::ws;
            v.v.push_back( x );
         }
         v.normalize();
         normals.push_back( v );
      }*/
      else if( key == "f" ) { // face
         Velvet::Face f{}; int v, t, n;
         while( !stringstream.eof() ) {
            stringstream >> v >> std::ws;
            f.vertex.push_back( v - 1 );
            if( stringstream.peek() == '/' ) {
               stringstream.get();
               if( stringstream.peek() == '/' ) {
                  stringstream.get();
                  stringstream >> n >> std::ws;
                  f.normals.push_back( n - 1 );
               }
               else {
                  stringstream >> t >> std::ws;
                  f.texture.push_back( t - 1 );
                  if( stringstream.peek() == '/' ) {
                     stringstream.get();
                     stringstream >> n >> std::ws;
                     f.normals.push_back( n - 1 );
                  }
               }
            }
         }
         m_faces.push_back( f );
      }
      else {
         //Logger::warn( "Unknown parameter encountered while parsing OBJ " + path );
      }
   }
   Logger::success( "WaveModel loaded with success. " );
   Logger::info( "VCount: " + std::to_string( getVertCount() ) );
   Logger::info( "FCount: " + std::to_string( getFaceCount() ) );
}

const size_t Velvet::Parser::WaveModel::getVertCount() const
{
    return m_verts.size();
}

const size_t Velvet::Parser::WaveModel::getFaceCount() const
{
    return m_faces.size();
}

const Velvet::Vertex& Velvet::Parser::WaveModel::getVertAt( size_t idx ) const
{
    return m_verts.at( idx );
}

const Velvet::Face& Velvet::Parser::WaveModel::getFaceAt( size_t idx ) const
{
    return m_faces.at( idx );
}