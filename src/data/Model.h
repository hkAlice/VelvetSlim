#ifndef _MODEL
#define _MODEL

#include <vector>

#include "Vec3.h"

namespace Velvet
{
   using Vertex = Velvet::Vec3;

   struct Face
   {
      std::vector< int > vertex;
      std::vector< int > texture;
      std::vector< int > normals;

      //Face( const Face& ) = delete;
   };
}

#endif // _MODEL