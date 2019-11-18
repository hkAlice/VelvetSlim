#include <algorithm>
#include <cstring>
#include <limits>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include "Renderer.h"
#include "ColorUtils.h"
#include "../parser/WaveModel.h"
#include "../logger/Logger.h"

using namespace Velvet;

Vec4Int& Velvet::Renderer::getClipMask()
{
    return m_clipMask;
}

void Velvet::Renderer::setClipMode( bool mode )
{
    m_isClipping = mode;
}

void Velvet::Renderer::setClipInvert( bool invert )
{
    m_clipInvert = invert;
}

bool Velvet::Renderer::getClipMode() const
{
    return m_isClipping;
}

bool Velvet::Renderer::getClipInvert() const
{
    return m_clipInvert;
}