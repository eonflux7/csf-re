// Game heap. Memory that crosses the boundary between original and reimplemented code (objects either side may
// free) MUST come from here: derive game classes from GameAlloc, and use legacy:: containers.
// Our own private data (hook infrastructure, tooling) may use the normal DLL heap.
#pragma once
#include <cstddef>
#include "globals.h"

namespace game {
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x6910F0, (size_t size));          // operator new
CSF_FUNC(void, __cdecl, Mem_Free, 0x691110, (void* p));                // operator delete
CSF_FUNC(void*, __cdecl, Mem_AllocTagged, 0x691120, (size_t size, unsigned short tag));
}  // namespace game

struct GameAlloc {
    static void* operator new(size_t n) { return game::Mem_Alloc(n); }
    static void* operator new[](size_t n) { return game::Mem_Alloc(n); }
    static void operator delete(void* p) { game::Mem_Free(p); }
    static void operator delete[](void* p) { game::Mem_Free(p); }
};
