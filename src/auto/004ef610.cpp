#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the element array of the object embedded at self+0x1C (the array header
// holds the element count at -4), re-stamping each 32-byte element's vtable slot
// with 0x82D410, and clears the array pointer. Callee cleans nothing (__thiscall, ret 0).
static void __fastcall AreaMesh_Shutdown(void* self, void* /*edx*/)
{
    uint8_t* state = reinterpret_cast<uint8_t*>(self) + 0x1C;
    uint8_t* arrayData = CSF_AT(uint8_t*, state, 0x0);

    CSF_AT(uint32_t, state, 0x4) = 0;
    CSF_AT(uint32_t, state, 0x8) = 0;

    if (arrayData)
    {
        int elementCount = CSF_AT(int, arrayData, -4);
        uint8_t* slot = arrayData + 32 * elementCount + 8;
        int remaining = elementCount - 1;
        if (remaining >= 0)
        {
            int left = remaining + 1;
            do
            {
                slot -= 0x20;
                --left;
                CSF_AT(void**, slot, 0) = CSF_VTBL(0x0082D410);
            } while (left);
        }

        Mem_Free(arrayData - 4);
        CSF_AT(uint32_t, state, 0x0) = 0;
    }
}

HOOK(0x004EF610, AreaMesh_Shutdown, "auto");
