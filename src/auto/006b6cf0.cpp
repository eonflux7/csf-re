#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Releases the per-entry objects of the 28-byte record array (slot +0x10 and +0x14 of each
// record, each released through its vtable slot 1), then frees the array block itself.
static char __fastcall WorldBuffers_ReleaseAll(S_006b6cf0* self, void* /*edx*/)
{
    uint32_t recordIndex = 0;
    uint32_t recordOffset = 0;

    if (self->field_001c)
    {
        do
        {
            // The record base is re-read from the struct on every access, as the original does.
            uint32_t attachedObjA = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), recordOffset + 0x14);
            if (attachedObjA)
            {
                void* objA = reinterpret_cast<void*>(attachedObjA);
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, objA, 0)[1])(objA, nullptr, 1);
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), recordOffset + 0x14) = 0;
            }

            uint32_t attachedObjB = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), recordOffset + 0x10);
            if (attachedObjB)
            {
                void* objB = reinterpret_cast<void*>(attachedObjB);
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, objB, 0)[1])(objB, nullptr, 1);
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), recordOffset + 0x10) = 0;
            }

            recordOffset += 28;
            ++recordIndex;
        } while (recordIndex < self->field_001c);
    }

    uint32_t blockPointer = self->field_0014;
    self->field_0018 = 0;
    self->field_001c = 0;
    if (blockPointer)
    {
        // LOBYTE(v5) = CoreMemory_Free(v5): only the low byte takes the free result.
        uint8_t freeResultLow = static_cast<uint8_t>(CoreMemory_Free(reinterpret_cast<void*>(blockPointer)));
        blockPointer = (blockPointer & 0xFFFFFF00u) | freeResultLow;
        self->field_0014 = 0;
    }
    self->field_0024 = 0;
    return static_cast<char>(blockPointer);
}

HOOK(0x006B6CF0, WorldBuffers_ReleaseAll, "auto");
