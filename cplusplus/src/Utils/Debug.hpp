#pragma once
#include <iostream>

// Taken from: https://stackoverflow.com/a/56534748/5181524
#ifdef NDEBUG
#define ASSERT(condition, message) 0
#else
#ifndef ASSERT
#define ASSERT(condition, message)\
   (!(condition)) ?\
      (std::cerr << "Assertion failed: (" << #condition << "), "\
      << "function " << __FUNCTION__\
      << ", file " << __FILE__\
      << ", line " << __LINE__ << "."\
      << std::endl << message << std::endl, abort(), 0) : 1
#endif
#endif