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

CSF_FUNC(void, __fastcall, C_00862490_ReleaseOwnedHandles, 0x006F1AC0, (C_00862490* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Vector deleting destructor for C_00862490 (144-byte elements).
// flags bit 1 (0x2): array form; element count is stored in the dword just before the object.
// flags bit 0 (0x1): release the memory block after destruction.
static char* __fastcall C_00862490_VectorDeletingDestructor(C_00862490* self, void* /*edx*/, char flags)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    if ((flags & 2) != 0)
    {
        int elementCount = CSF_AT(int, self, -4);
        uint8_t* blockStart = base - 4;
        uint8_t* cursor = base + elementCount * 0x90;

        int remaining = elementCount - 1;
        if (remaining >= 0)
        {
            int iterations = remaining + 1;
            do
            {
                cursor -= 0x90;
                C_00862490_ReleaseOwnedHandles(reinterpret_cast<C_00862490*>(cursor), nullptr);
                --iterations;
            } while (iterations != 0);
        }

        if ((flags & 1) != 0)
            CoreMemory_Free(blockStart);

        return reinterpret_cast<char*>(blockStart);
    }

    C_00862490_ReleaseOwnedHandles(self, nullptr);

    if ((flags & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<char*>(base);
}

HOOK(0x006CA2C0, C_00862490_VectorDeletingDestructor, "auto");
