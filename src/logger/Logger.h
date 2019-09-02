#ifndef _LOGGER
#define _LOGGER

#include <iostream>
#include <sstream>
#include <string>
#include "../../libs/termcolor/termcolor.hpp"

namespace Logger
{
    template<class T>
    static void error(const T& output)
    {
        std::cout << "[ " << termcolor::blink << termcolor::red << "FAIL" << termcolor::reset << " ] ";
        std::cout << output << std::endl;
    }

    template<class T>
    static void info(const T& output)
    {
        std::cout << "[ " << termcolor::cyan << "INFO" << termcolor::reset << " ] ";
        std::cout << output << std::endl;
    }

    template<class T> 
    static void warn(const T& output)
    {
        std::cout << "[ " << termcolor::yellow << "WARN" << termcolor::reset << " ] ";
        std::cout << output << std::endl;
    }

    template<class T> 
    static void success(const T& output)
    {
        std::cout << "[ " << termcolor::green << "PASS" << termcolor::reset << " ] ";
        std::cout << output << std::endl;
    }
};

#endif // _LOGGER