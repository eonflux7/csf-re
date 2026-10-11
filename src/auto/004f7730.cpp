#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees a counted array of 48-byte elements whose header (element count) sits
// 4 bytes before the first element. Each element's two vtable slots are reset
// to C_0082d410's vtable before the block is released, then the owning pointer is cleared.
static void __fastcall CharacterModel_FreeTaggedArray(int* self, void* /*edx*/)
{
    uint8_t* arrayBase = reinterpret_cast<uint8_t*>(*self);
    if (arrayBase == nullptr)
        return;

    int elementCount = CSF_AT(int, arrayBase, -4);
    void* vtbl = CSF_VTBL(0x0082D410);

    for (int index = elementCount - 1; index >= 0; --index)
    {
        uint8_t* element = arrayBase + 48 * index;
        CSF_AT(void*, element, 0x10) = vtbl;
        CSF_AT(void*, element, 0x04) = vtbl;
    }

    Mem_Free(arrayBase - 4);
    *self = 0;
}

HOOK(0x004F7730, CharacterModel_FreeTaggedArray, "auto");
