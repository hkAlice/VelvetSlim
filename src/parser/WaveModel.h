#ifndef _WAVEMODELPARSER
#define _WAVEMODELPARSER

#include <string>
#include <vector>

#include "../data/Model.h"
#include "../data/Vec3.h"

namespace Velvet
{
namespace Parser
{
    class WaveModel
    {
    public:
        // TODO: I'd like to handle resource acquisition separate from
        // the parser responsability
        // TODO: Separate parsing from model container, polymorphism
        
        WaveModel( const std::string& path );
        ~WaveModel(){};

        const Vertex& getVertAt( size_t idx ) const;
        const Face& getFaceAt( size_t idx ) const;

        // todo: reference
        std::vector< Vertex >& getVerts() const;

        const size_t getVertCount() const;
        const size_t getFaceCount() const;
    
    private:
        std::vector< Vertex > m_verts;
        // TODO: structure
        std::vector< Face > m_faces;

    };
}

}

#endif // _WAVEMODELPARSER