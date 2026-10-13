#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Mem_FreeDebug: debug heap free, cdecl, caller pops.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases a growable array of 0xC-byte polymorphic elements stored at
// self+4 (begin), self+8 (end), self+12 (capacity). Each element's first
// dword is a vtable; slot 0 is invoked as a destructor-like call with arg 0.
// Finally frees the array buffer and zeroes the three pointers.
// Returns 0. __thiscall, callee pops nothing (ret 0).
static int __fastcall ReleaseEmbeddedMember(void* self, void* /*edx*/)
{
    uint8_t* element = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));
    if (element)
    {
        uint8_t* end = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
        for (; element != end; element += 0x0C)
        {
            // Virtual slot 0 (byte offset 0) of the element, called with arg 0.
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, element, 0)[0])(element, nullptr, 0);
        }

        if (CSF_AT(uint32_t, self, 0x04))
        {
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x04)));
        }
    }

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return 0;
}

HOOK(0x0044AFF0, ReleaseEmbeddedMember, "auto");
