#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int SceneLightsObjects_DestroyArray(uint32_t arrayAddress)
{
    void* allocation = reinterpret_cast<void*>(arrayAddress - 4);
    uint32_t count = CSF_AT(uint32_t, allocation, 0);

    if (static_cast<int32_t>(count - 1) >= 0)
    {
        do
        {
            --count;
            void* element = reinterpret_cast<void*>(arrayAddress + count * 0x28);
            CSF_AT(void**, element, 8) = CSF_VTBL(0x0082D410);
        } while (count != 0);
    }

    return Mem_Free(allocation);
}

static int __fastcall C_0083ef30_Destroy(C_0083ef30* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083EF30);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083EF10));

    uint32_t arrayAddress = self->field_002c;
    if (arrayAddress != 0)
    {
        SceneLightsObjects_DestroyArray(arrayAddress);
        self->field_002c = 0;
    }

    arrayAddress = self->field_001c;
    if (arrayAddress != 0)
    {
        SceneLightsObjects_DestroyArray(arrayAddress);
        self->field_001c = 0;
    }

    int result = 0;
    arrayAddress = self->field_000c;
    if (arrayAddress != 0)
    {
        result = SceneLightsObjects_DestroyArray(arrayAddress);
        self->field_000c = 0;
    }

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004F0E80, C_0083ef30_Destroy, "auto");
