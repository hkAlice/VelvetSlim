/*#ifndef _LOGGER
#define _LOGGER

#include <iostream>
#include <sstream>
#include <string>
#include "../../libs/termcolor/termcolor.hpp"

enum LogLevel
    {
        INFO,
        WARN,
        ERROR
    };

class Logger
{
public:
    typedef std::ostream&  (*ManipFn)(std::ostream&);
    typedef std::ios_base& (*FlagsFn)(std::ios_base&);

    Logger() : m_logLevel(INFO) {}

    template<class T>  // int, double, strings, etc
    Logger& operator<<(const T& output)
    {
        m_stream << output;
        return *this;
    }

    Logger& operator<<(ManipFn manip) /// endl, flush, setw, setfill, etc.
    { 
        manip(m_stream);

        if (manip == static_cast<ManipFn>(std::flush)
         || manip == static_cast<ManipFn>(std::endl ) )
            this->flush();

        return *this;
    }

    Logger& operator<<(FlagsFn manip) /// setiosflags, resetiosflags
    {
        manip(m_stream);
        return *this;
    }

    Logger& operator()(LogLevel e)
    {
        m_logLevel = e;

        switch(e)
        {
            case LogLevel::INFO:
            {
                m_stream << "[" << termcolor::red << "  INFO " << termcolor::reset << "]";
                break;
            }
            case LogLevel::WARN:
            {
                m_stream << "[" << termcolor::yellow << "  WARN " << termcolor::reset << "]";
                break;
            }
            case LogLevel::ERROR:
            {
                m_stream << "[" << termcolor::blink << termcolor::red << " ERROR " << termcolor::reset << "]";
                break;
            }
            
        }
        
        return *this;
    }

    void flush() 
    {

        std::cout << m_stream.str();

        m_logLevel = INFO;

        m_stream.str( std::string() );
        m_stream.clear();
    }

private:
    std::stringstream  m_stream;
    int                m_logLevel;
};

#endif // _LOGGER*/