#ifndef _VEC2INT
#define _VEC2INT

#include <cstdint>
#include <cmath>

namespace Velvet
{
    struct Vec2Int
    {
        bool operator==( Vec2Int& rhs ) const
        {
            return ( x == rhs.x && y == rhs.y );
        }

        Vec2Int operator+( Vec2Int& rhs ) const
        {
            return { x + rhs.x, y + rhs.y };
        }

        Vec2Int operator-( Vec2Int& rhs ) const
        {
            return { x - rhs.x, y - rhs.y };
        }

        Vec2Int operator*( float scalar ) const
        {
            return { static_cast<int64_t>( x * scalar ), static_cast<int64_t>( y * scalar ) };
        }
        Vec2Int operator/( float scalar ) const
        {
           return { static_cast<int64_t>( x / scalar ), static_cast<int64_t>( y / scalar ) };
        }

        int64_t dot( Vec2Int& rhs ) const
        {
            return { x * rhs.x + y * rhs.y };
        }

        float length() const
        {
            return float( std::sqrt( x * x + y * y ) );
        }

        static void swap( Vec2Int& A, Vec2Int& B )
        {
            Vec2Int tmp = { A.x, A.y };
            A.x = B.x;
            A.y = B.y;
            
            B.x = tmp.x;
            B.y = tmp.y;
        }

        int64_t x;
        int64_t y;
    };
}

#endif // _VEC2INT