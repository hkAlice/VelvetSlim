#ifndef _VEC4INT
#define _VEC4INT

#include <cstdint>
#include <cmath>

namespace Velvet
{
    struct Vec4Int
    {   
        // TODO: add these op overloads
        /*
        bool operator==( Vec4Int& rhs ) const
        {
            return ( x == rhs.x && y == rhs.y );
        }

        Vec4Int operator+( Vec4Int& rhs ) const
        {
            return { x + rhs.x, y + rhs.y };
        }

        Vec4Int operator-( Vec4Int& rhs ) const
        {
            return { x - rhs.x, y - rhs.y };
        }
'
        
        Vec4Int operator*( float scalar ) const
        {
            return { static_cast<int64_t>( x * scalar ), static_cast<int64_t>( y * scalar ) };
        }
        Vec4Int operator/( float scalar ) const
        {
           return { static_cast<int64_t>( x / scalar ), static_cast<int64_t>( y / scalar ) };
        }

        int64_t dot( Vec4Int& rhs ) const
        {
            return { x * rhs.x + y * rhs.y };
        }

        float length() const
        {
            return float( std::sqrt( x * x + y * y ) );
        }

        static void swap( Vec4Int& A, Vec4Int& B )
        {
            Vec4Int tmp = { A.x, A.y };
            A.x = B.x;
            A.y = B.y;
            
            B.x = tmp.x;
            B.y = tmp.y;
        }
        */


        int64_t x;
        int64_t y;
        int64_t z;
        int64_t w;
    };
}

#endif // _Vec4Int