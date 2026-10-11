#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(char, __stdcall, GfxSkin_DestroyEntries, 0x006B4DC0, (int a1, int a2));
CSF_FUNC(int, __fastcall, GfxSkin_ReleaseResources, 0x006C6E60, (void* self, void* edx));

static void __fastcall C_008608e8_Destroy(C_008608e8* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008608E8);
    GfxSkin_ReleaseResources(self, nullptr);

    if (self->field_0004)
    {
        void* object = reinterpret_cast<void*>(self->field_0004);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
        self->field_0004 = 0;
    }

    if (self->field_013c)
    {
        GfxSkin_DestroyEntries(
            static_cast<int>(self->field_013c),
            CSF_AT(int, self, 0x140));
        if (self->field_013c)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_013c));
    }
    self->field_013c = 0;
    CSF_AT(std::uint32_t, self, 0x140) = 0;
    CSF_AT(std::uint32_t, self, 0x144) = 0;

    if (self->field_00e0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00e0));
        self->field_00e0 = 0;
    }
    if (self->field_00d0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00d0));
        self->field_00d0 = 0;
    }
    if (self->field_00c0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00c0));
        self->field_00c0 = 0;
    }
    if (self->field_00b0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00b0));
        self->field_00b0 = 0;
    }
    if (self->field_00a0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_00a0));
        self->field_00a0 = 0;
    }
    if (self->field_0090)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0090));
        self->field_0090 = 0;
    }
    if (self->field_0080)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0080));
        self->field_0080 = 0;
    }
    if (self->field_0070)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0070));
        self->field_0070 = 0;
    }
    if (self->field_0060)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0060));
        self->field_0060 = 0;
    }
    if (self->field_0050)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0050));
        self->field_0050 = 0;
    }
    if (self->field_0040)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0040));
        self->field_0040 = 0;
    }
    if (self->field_0030)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0030));
        self->field_0030 = 0;
    }
    if (self->field_001c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    if (self->field_0018)
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_0014));

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006C7080, C_008608e8_Destroy, "auto");
