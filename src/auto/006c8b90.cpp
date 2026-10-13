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

// Mem_Free (cdecl): releases a block allocated by the game heap.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the two heap buffers held at self+0x50 and self+0x40 and nulls their slots.
// Walks the slots from the highest offset down, as the original does.
// Thiscall, no stack args, ret 0.
static int __fastcall ClearTwoOwnedBuffers(void* self, void* /*edx*/)
{
    int result = 0;
    int slot = 0x60;   // esi = this + 0x60 at entry
    int remaining = 2; // edi = 2

    do
    {
        void* block = CSF_AT(void*, self, slot - 0x10);
        result = static_cast<int>(reinterpret_cast<std::intptr_t>(block));
        slot -= 0x10;

        if (block != nullptr)
        {
            result = Mem_Free(block);
            CSF_AT(void*, self, slot) = nullptr;
        }

        --remaining;
    } while (remaining != 0);

    return result;
}

HOOK(0x006C8B90, ClearTwoOwnedBuffers, "auto");
