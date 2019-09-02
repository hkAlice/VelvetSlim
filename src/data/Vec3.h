#ifndef _VEC3
#define _VEC3

#include <cmath>

namespace Velvet
{
    struct Vec3
    {
        bool operator==( Vec3& rhs ) const
        {
            return { x == rhs.x && y == rhs.y && z == rhs.z };
        }

        Vec3 operator+( Vec3& rhs ) const
        {
            return { x + rhs.x, 
                     y + rhs.y, 
                     z + rhs.z };
        }

        Vec3 operator-( Vec3& rhs ) const
        {
            return { x - rhs.x, 
                     y - rhs.y, 
                     z - rhs.z };
        }

        Vec3 operator*( float scalar ) const
        {
            return { x * scalar, 
                     y * scalar,
                     z * scalar };
        }

        Vec3 operator/( float scalar ) const
        {
            return { x / scalar, 
                     y / scalar,
                     z / scalar };
        }

        Vec3 operator^( Vec3& rhs ) const
        {
            return { y * rhs.z - z * rhs.y, 
                     z * rhs.x - x * rhs.z,
                     x * rhs.y - y * rhs.x };
        }

        Vec3 cross( Vec3& rhs ) const
        {
            return { y * rhs.z - z * rhs.y,
                     z * rhs.x - x * rhs.z,
                     x * rhs.y - y * rhs.x };
        }

        float dot( Vec3& rhs ) const
        {
            return x * rhs.x + 
                   y * rhs.y + 
                   z * rhs.z;
        }

        float length() const
        {
            return float( std::sqrt( x*x + y*y + z*z ) );
        }

        float x;
        float y;
        float z;
    };
}

#endif // _VEC3