// Address-bound globals for the hook phase. At cutover (CSF_STANDALONE), these become real definitions.
#pragma once
#include <cstdint>

#ifndef CSF_STANDALONE
#define CSF_GLOBAL(type, name, addr) static auto& name = *reinterpret_cast<type*>(addr)
#else
#define CSF_GLOBAL(type, name, addr) extern type name
#endif
