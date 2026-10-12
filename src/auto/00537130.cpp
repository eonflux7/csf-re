#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00843720_ReleaseSubobjects(
    C_00843720* self, void* /*edx*/)
{
    const uint32_t ownedObject = CSF_AT(uint32_t, self, 0x13C);

    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00842038);
    CSF_AT(void**, self, 0x4) = CSF_VTBL(0x00841FE8);
    CSF_AT(void**, self, 0x10) = CSF_VTBL(0x00841FE4);

    if (ownedObject)
    {
        void* block = CSF_AT(void*, self, 0x14C);
        if (block)
        {
            Mem_Free(block);
            CSF_AT(uint32_t, self, 0x14C) = 0;
        }

        void* object = CSF_AT(void*, self, 0x13C);
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            CSF_AT(uint32_t, self, 0x13C) = 0;
        }
    }

    if (CSF_AT(uint32_t, self, 0x178) >= 0x10)
    {
        void* block = CSF_AT(void*, self, 0x164);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0x178) = 15;
    CSF_AT(uint32_t, self, 0x174) = 0;
    CSF_AT(uint8_t, self, 0x164) = 0;

    if (CSF_AT(uint32_t, self, 0x12C) >= 0x10)
    {
        void* block = CSF_AT(void*, self, 0x118);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0x128) = 0;
    CSF_AT(uint32_t, self, 0x12C) = 15;
    CSF_AT(uint8_t, self, 0x118) = 0;

    // Cleanup continuation at the original tail-jump destination.
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00830578);
    CSF_AT(void**, self, 0x4) = CSF_VTBL(0x00830528);
    CSF_AT(void**, self, 0x10) = CSF_VTBL(0x00830520);

    int result = 0;
    void* block = CSF_AT(void*, self, 0xE4);
    if (block)
    {
        result = Mem_Free(block);
        CSF_AT(uint32_t, self, 0xE4) = 0;
    }

    CSF_AT(void**, self, 0x4) = CSF_VTBL(0x00830488);
    CSF_AT(void**, self, 0x4) = CSF_VTBL(0x00830438);
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x0083042C);

    return result;
}

HOOK(0x00537130, C_00843720_ReleaseSubobjects, "auto");
