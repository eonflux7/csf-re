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

// Game heap free (cdecl): block pointer released by the emitter list.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases every emitter in the list (each deleted via its deleting destructor, vtable slot 0),
// clears the list and frees the backing array.
// Layout: [0] = item array pointer, [+4] = unused here (zeroed), [+8] = item count.
static int __fastcall ParticleEmitters_ReleaseAll(void* self, void* /*edx*/)
{
    const int itemCount = CSF_AT(int, self, 8);

    for (int i = 0; i < itemCount; ++i)
    {
        // The array pointer is re-read on every iteration: a deleted emitter may touch the list.
        uint8_t* itemSlots = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0));
        uint8_t* slot = itemSlots + 4u * static_cast<unsigned>(i);
        void* emitter = reinterpret_cast<void*>(CSF_AT(uint32_t, slot, 0));

        if (emitter != nullptr)
        {
            // Deleting destructor: vtable slot 0, called with (this, 1).
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, emitter, 0)[0])(emitter, nullptr, 1);

            itemSlots = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0));
            slot = itemSlots + 4u * static_cast<unsigned>(i);
            CSF_AT(uint32_t, slot, 0) = 0;
        }
    }

    const uint32_t oldArray = CSF_AT(uint32_t, self, 0);
    int result = static_cast<int>(oldArray);

    CSF_AT(uint32_t, self, 4) = 0;
    CSF_AT(uint32_t, self, 8) = 0;

    if (oldArray != 0)
    {
        result = Mem_Free(reinterpret_cast<void*>(oldArray));
        CSF_AT(uint32_t, self, 0) = 0;
    }

    return result;
}

HOOK(0x6E08D0, ParticleEmitters_ReleaseAll, "auto");
