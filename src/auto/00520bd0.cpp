#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __fastcall, C_00840fdc_ClearSceneResources, 0x00520420, (C_00840fdc* self, void* , char clearOptionalResources));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00840fdc_Destroy(C_00840fdc* self, void* /*edx*/)
{
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x00840FDC));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00840FBC));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00840FB4));

    C_00840fdc_ClearSceneResources(self, nullptr, 1);

    for (uint32_t index = 0; index < self->field_0014; ++index)
    {
        void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), index * 4);
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), index * 4) = 0;
        }
    }

    const uint32_t initialSecondaryCount = self->field_0024;
    self->field_0014 = 0;
    if (initialSecondaryCount)
    {
        uint32_t index = 0;
        do
        {
            void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_001c), index * 4);
            if (object)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[2])(object, nullptr, 1);
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_001c), index * 4) = 0;
            }
            ++index;
        }
        while (index < self->field_0024);
    }

    self->field_0024 = 0;
    Hud_UnregisterObject(reinterpret_cast<int>(self));

    void* auxiliaryObject = reinterpret_cast<void*>(self->field_0090);
    if (auxiliaryObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, auxiliaryObject, 0)[0])(auxiliaryObject, nullptr, 1);
        self->field_0090 = 0;
    }

    if (self->field_007c)
    {
        auto* elements = reinterpret_cast<uint8_t*>(self->field_007c);
        void* allocation = elements - 4;
        const uint32_t count = CSF_AT(uint32_t, allocation, 0);
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            for (uint32_t remaining = count; remaining != 0; --remaining)
            {
                CSF_AT(void**, elements + (remaining - 1) * 0x18, 4) =
                    CSF_VTBL(0x0082D410);
            }
        }
        Mem_Free(allocation);
        self->field_007c = 0;
    }

    if (self->field_006c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_006c));
        self->field_006c = 0;
    }

    if (self->field_005c)
    {
        auto* elements = reinterpret_cast<uint8_t*>(self->field_005c);
        void* allocation = elements - 4;
        const uint32_t count = CSF_AT(uint32_t, allocation, 0);
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            for (uint32_t remaining = count; remaining != 0; --remaining)
            {
                CSF_AT(void**, elements + (remaining - 1) * 0x24, 4) =
                    CSF_VTBL(0x0082D410);
            }
        }
        Mem_Free(allocation);
        self->field_005c = 0;
    }

    if (self->field_004c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_004c));
        self->field_004c = 0;
    }
    if (self->field_003c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_003c));
        self->field_003c = 0;
    }
    if (self->field_002c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_002c));
        self->field_002c = 0;
    }
    if (self->field_001c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    int result = static_cast<int>(self->field_000c);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00840F94));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    return result;
}

HOOK(0x00520BD0, C_00840fdc_Destroy, "auto");
