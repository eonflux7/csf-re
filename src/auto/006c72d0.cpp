#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicDestroy, 0x0058F060, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpAtomicCreate, 0x0058F5C0, ());
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(uint32_t*, __cdecl, GfxSkin_GetSharedData, 0x006C8370, ());

CSF_GLOBAL(uint32_t, g_DefaultAtomicCallback, 0x008BB4B4);

static int __fastcall C_008608e8_Construct(C_008608e8* self, void* /*edx*/)
{
    C_00860178_Construct(static_cast<C_00860178*>(self), nullptr);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008608E8);
    self->field_0008 = 0;
    self->field_0014 = reinterpret_cast<uint32_t>(RwMatrixCreate());
    self->field_0018 = 1;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 10;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 10;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 10;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 10;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 10;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 10;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 10;
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 10;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 10;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 10;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 10;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 0;
    self->field_00dc = 10;
    self->field_00e0 = 0;
    self->field_00e4 = 0;
    self->field_00e8 = 0;
    self->field_00ec = 10;
    self->field_00f8 = 0;
    self->field_00fc = 0;
    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_0108 = 0;
    self->field_010c = 0;
    self->field_0114 = 0;
    self->field_0118 = 0;
    self->field_011c = 0;
    self->field_0120 = 0;
    self->field_0110 = 0;
    self->field_0124 = 0;
    self->field_0128 = 0;
    self->field_012c = 0;
    self->field_0130 = 1065353216;
    self->field_0134 = 1;
    self->field_013c = 0;
    self->field_0140 = 0;
    self->field_0144 = 0;
    self->field_00f5 = 0;
    self->field_0004 = reinterpret_cast<uint32_t>(GfxSkin_GetSharedData());

    uint32_t* atomic = reinterpret_cast<uint32_t*>(RpAtomicCreate());
    g_DefaultAtomicCallback = CSF_AT(uint32_t, atomic, 0x48);
    RpAtomicDestroy(atomic);

    self->field_0008 = -1;
    self->field_0008 &= ~0x8000u;
    self->field_00f0 = 0;
    self->field_002c = 0;
    self->field_00f4 = 0;
    self->field_000c = 0;
    self->field_0010 = 20;
    self->field_0028 = 50;
    return reinterpret_cast<int>(self);
}

HOOK(0x006C72D0, C_008608e8_Construct, "auto");
