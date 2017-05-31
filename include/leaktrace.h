#ifndef __LEAK_TRACE_H_
#define __LEAK_TRACE_H_
#include <cstddef>

// Overload operator new
void* operator new(std::size_t, const char*, long);
void* operator new[](std::size_t, const char*, long);
#define new new (__FILE__, __LINE__)

extern bool traceFlag;
#define TRACE_ON() traceFlag = true
#define TRACE_OFF() traceFlag = false

extern bool activeFlag;
#define MEM_ON() activeFlag = true 
#define MEM_OFF() activeFlag = false

#endif
