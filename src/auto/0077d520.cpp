#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callee: element destructor/reset, __fastcall-style per packet bind line.
CSF_FUNC(char, __fastcall, sub_778140, 0x00778140, (void* self, void* edx));

// Game heap free with debug info (__cdecl, one argument).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static constexpr std::size_t kElementStride = 0x48;

// Destroys every element in the [begin, end) range (stride 0x48), frees the
// backing block if one exists, and clears the three list pointers.
static int __fastcall ResetElementList(void* self, void* /*edx*/)
{
    uint8_t* cursor = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04)));
    if (cursor)
    {
        uint8_t* end = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
        for (; cursor != end; cursor += kElementStride)
        {
            sub_778140(cursor, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x04) != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04))));
        }
    }

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return 0;
}

HOOK(0x0077D520, ResetElementList, "auto");
