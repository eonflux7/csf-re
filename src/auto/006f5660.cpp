#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, GfxCollision_Reset, 0x006EC2A0, (void* self, void*));

// Releases every entry of the pointer array at field_0004 (each entry's vtable slot 0 is
// called with arg 1), clears count fields, frees the array block, then resets the
// embedded collision object at +0x14.
static char __fastcall C_008627d8_ReleaseSlotsAndReset(C_008627d8* self, void* /*edx*/)
{
    uint32_t slotCount = self->field_000c;
    for (uint32_t i = 0; i < slotCount; ++i)
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0004)[i];
        if (entry != 0)
        {
            void* obj = reinterpret_cast<void*>(entry);
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0004)[i] = 0;
        }
    }

    uint32_t slotArray = self->field_0004;
    self->field_0008 = 0;
    self->field_000c = 0;
    if (slotArray != 0)
    {
        Mem_Free(reinterpret_cast<void*>(slotArray));
        self->field_0004 = 0;
    }

    return GfxCollision_Reset(&self->field_0014, nullptr);
}

HOOK(0x006F5660, C_008627d8_ReleaseSlotsAndReset, "auto");
