#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_500D60, 0x00500D60, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall CSceneLoaderBase_Destroy(CSceneLoaderBase* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083F578);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0083F574));

    if (CSF_AT(uint32_t, self, 0xD0) >= 8 &&
        CSF_AT(uint32_t, self, 0xBC) != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0xBC)));
    }
    CSF_AT(uint32_t, self, 0xD0) = 7;
    CSF_AT(uint32_t, self, 0xCC) = 0;
    CSF_AT(uint16_t, self, 0xBC) = 0;

    if (CSF_AT(uint32_t, self, 0xB0) >= 0x10 &&
        CSF_AT(uint32_t, self, 0x9C) != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x9C)));
    }
    CSF_AT(uint32_t, self, 0xB0) = 15;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    CSF_AT(uint8_t, self, 0x9C) = 0;

    if (CSF_AT(uint32_t, self, 0x94) >= 0x10 &&
        CSF_AT(uint32_t, self, 0x80) != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x80)));
    }
    CSF_AT(uint32_t, self, 0x94) = 15;
    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint8_t, self, 0x80) = 0;

    if (CSF_AT(uint32_t, self, 0x78) >= 0x10 &&
        CSF_AT(uint32_t, self, 0x64) != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x64)));
    }
    CSF_AT(uint32_t, self, 0x78) = 15;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint8_t, self, 0x64) = 0;

    if (CSF_AT(uint32_t, self, 0x3C) != 0)
    {
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x3C)));
        CSF_AT(uint32_t, self, 0x3C) = 0;
    }

    int result = CSF_AT(int, self, 0x30);
    if (result != 0)
    {
        sub_500D60(
            result,
            CSF_AT(int, self, 0x34),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x2C),
            0);

        result = CSF_AT(int, self, 0x30);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083F4BC));
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x005035D0, CSceneLoaderBase_Destroy, "auto");
