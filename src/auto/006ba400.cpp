#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (cdecl, caller pops the argument).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Registers a property node with the owner; __thiscall, arg on stack, callee pops 4.
CSF_FUNC(char, __fastcall, sub_6BA300, 0x006BA300, (void* self, void* edx, uint32_t* a3));

// Allocates a 20-byte property record, initializes it, and hands it to the owner.
// Returns the new record, or nullptr if the allocation failed.
static uint32_t* __fastcall GfxObjectProps_CreatePropertyEntry(S_006b9580* self, void* /*edx*/)
{
    uint32_t* entry = nullptr;
    void* allocated = Mem_Alloc(20);
    if (allocated != nullptr)
    {
        CSF_AT(uint32_t, allocated, 0x0C) = 0;
        CSF_AT(uint8_t, allocated, 0x01) = 0;
        CSF_AT(uint8_t, allocated, 0x00) = 0;
        CSF_AT(uint32_t, allocated, 0x08) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, allocated, 0x04) = 0;
        CSF_AT(uint8_t, allocated, 0x10) = 0;
        CSF_AT(uint8_t, allocated, 0x11) = 0;
        entry = reinterpret_cast<uint32_t*>(allocated);
    }

    alignas(8) uint32_t registerArgs[2];
    registerArgs[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(entry));
    registerArgs[1] = 1;
    sub_6BA300(self, nullptr, registerArgs);

    return entry;
}

HOOK(0x006BA400, GfxObjectProps_CreatePropertyEntry, "auto");
