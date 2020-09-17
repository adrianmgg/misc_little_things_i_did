#pragma once
#ifndef _AMGG_LAZYPRINT
#define _AMGG_LAZYPRINT

#include <iostream>

#define __LAZYPRINT_STRINGIFY_DIRECT(x) #x
#define __LAZYPRINT_STRINGIFY_INDIRECT(x) __LAZYPRINT_STRINGIFY_DIRECT(x)
#define lazyPrint(x) std::cout << __FILE__ ":" __LAZYPRINT_STRINGIFY_INDIRECT(__LINE__) "  " #x " = " << (x) << std::endl

#endif