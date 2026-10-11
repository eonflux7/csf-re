#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, DestroyEmbeddedObject, 0x004DB3C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the four embedded sub-objects (at +0x3C, +0x28, +0x14, +0x00), then
// frees the block itself when bit 0 of the argument is set. Returns `this`.
static char* __fastcall TFreeMove_Destroy(TFreeMove* self, void* /*edx*/, char freeMemory)
{
    DestroyEmbeddedObject(reinterpret_cast<uint8_t*>(self) + 0x3C, nullptr);
    DestroyEmbeddedObject(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);
    DestroyEmbeddedObject(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    DestroyEmbeddedObject(self, nullptr);

    if ((freeMemory & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x004215E0, TFreeMove_Destroy, "auto");
