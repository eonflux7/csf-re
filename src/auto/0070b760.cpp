#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* /*edx*/));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* /*edx*/, int a2, int a3));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* /*edx*/, int key, int value));
CSF_FUNC(char, __cdecl, GfxSkin_ListHasUnresolvedEntry, 0x006C4D50, (int a1));

static int __fastcall C_008636fc_Construct(C_008636fc* self, void* /*edx*/, int a2, int a3)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    *(void***)self = CSF_VTBL(0x008636FC);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    self->field_0014 = static_cast<uint32_t>(a2);
    self->field_0018 = static_cast<uint32_t>(a3);
    self->field_0020 = 1;

    uint32_t subObject = reinterpret_cast<uint32_t>(Mem_Alloc(28));
    if (subObject)
    {
        C_00860178_Construct(reinterpret_cast<C_00860178*>(subObject), nullptr);
        *(void***)subObject = CSF_VTBL(0x0086370C);
        C_00860894_Init(reinterpret_cast<void*>(subObject + 8), nullptr, -1, 0);
        CSF_AT(uint32_t, subObject, 4) = reinterpret_cast<uint32_t>(self);
    }
    else
    {
        subObject = 0;
    }
    self->field_001c = subObject;

    if (GfxSkin_ListHasUnresolvedEntry(a3))
    {
        void* holder = reinterpret_cast<void*>(self->field_001c);
        C_008670f0* entry = reinterpret_cast<C_008670f0*(__fastcall*)(void*, void*)>(CSF_AT(void**, holder, 0)[7])(holder, nullptr);
        C_008670f0_SetEntry(entry, nullptr, 2, a3);
    }
    return reinterpret_cast<int>(self);
}

HOOK(0x0070B760, C_008636fc_Construct, "auto");
