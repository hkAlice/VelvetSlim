#ifndef _VEC2
#define _VEC2

#include <cmath>

namespace Velvet
{
    struct Vec2 
    {
        bool operator==( Vec2& rhs ) const
        {
            return ( x == rhs.x && y == rhs.y );
        }

        Vec2 operator+( Vec2& rhs ) const
        {
            return { x + rhs.x, y + rhs.y };
        }

        Vec2 operator-( Vec2& rhs ) const
        {
            return { x - rhs.x, y - rhs.y };
        }

        Vec2 operator*( int scalar ) const
        {
            return { x * scalar, y * scalar };
        }
        Vec2 operator/( int scalar ) const
        {
            return { x / scalar, y / scalar };
        }

        float dot( Vec2& rhs ) const
        {
            return { x * rhs.x + y * rhs.y };
        }

        float length() const
        {
            return float( std::sqrt( x * x + y * y ) );
        }

        float x;
        float y;
    };
}

#endif // _VEC2