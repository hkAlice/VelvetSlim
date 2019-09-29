#include "Object.h"

Velvet::Object::Object()
{

}

void Velvet::Object::setWorkFunction( std::function<void()> workFunc )
{
    m_workFunc = workFunc;
}

const void Velvet::Object::render()
{
    m_workFunc();
}