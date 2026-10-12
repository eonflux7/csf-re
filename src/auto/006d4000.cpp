#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Member initializer for the new shape registry (__fastcall: ecx=this, edx unused).
CSF_FUNC(uint32_t*, __fastcall, sub_6D5400, 0x006D5400, (void* self, void* edx));

static uint32_t* __cdecl ClumpDict_CreateShapeRegistry()
{
    void* registry = Mem_Alloc(32);
    if (!registry)
        return nullptr;

    // The original tail-jumps into 0x6FD810, which runs this body inline:
    // initialize the members, then install the registry vtable.
    sub_6D5400(registry, nullptr);
    *(void***)registry = CSF_VTBL(0x00862FCC);
    return static_cast<uint32_t*>(registry);
}

HOOK(0x006D4000, ClumpDict_CreateShapeRegistry, "auto");
