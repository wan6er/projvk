#pragma once
#include "doctest.h"
// #define TEST_FUNC(name) TEST_CASE(__FILE__ name)

#define TEST_FUNC_BEGIN(name) \
TEST_CASE(name) { \
do { std::cout << "testing [" << name << "]..."; } while(false); \

#define TEST_FUNC_END do { std::cout << "finished\n"; } while (false); }