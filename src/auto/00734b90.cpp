#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(char, __fastcall, GrowSharedStorage, 0x00732D00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GrowChildStorage, 0x00732DA0, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_SharedStorage, 0x008A88BC);
CSF_GLOBAL(uint32_t, g_SharedStorageCount, 0x008A88C0);
CSF_GLOBAL(uint32_t, g_SharedStorageLimit, 0x008A88C4);

static uint32_t* __fastcall C_008670f0_InitializeChild(
    C_008670f0* self, void* /*edx*/, int parent)
{
    *(void***)self = CSF_VTBL(0x008670F0);
    self->field_0034 = 0;
    self->field_0038 = 7;
    *reinterpret_cast<uint16_t*>(&self->field_0024) = 0;
    self->field_0054 = 7;
    self->field_0050 = 0;
    *reinterpret_cast<uint16_t*>(&self->field_0040) = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 10;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 10;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 10;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 10;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 1.0f;
    self->field_00c0 = 0.0f;
    self->field_00c4 = 0.0f;
    self->field_00dc = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    self->field_00e4 = 0;
    CSF_AT(uint32_t, self, 0xE8) = 10;
    self->field_00ec = 0.0f;
    self->field_00f0 = 0.0f;

    self->field_0004 = parent;
    void* parentObject = reinterpret_cast<void*>(parent);
    self->field_0008 =
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, parentObject, 0)[0x68 / 4])(
                 parentObject, nullptr);

    self->field_00a8 = 1132396544;
    self->field_00ac = 1132396544;
    self->field_00b0 = 1132396544;
    self->field_00c0 = 1.0f;
    self->field_00c4 = 1.0f;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00b4 = 255.0f;
    self->field_0074 = 1;
    self->field_0064 = 1;
    self->field_0094 = 1;
    self->field_0084 = 1;

    if (g_SharedStorageCount < 0x23)
    {
        SetAllocationMode(3);
        if (g_SharedStorageCount < 0x23)
            GrowSharedStorage(&g_SharedStorage, nullptr,
                              35 - g_SharedStorageCount);
        g_SharedStorageLimit = 35;
        SetAllocationMode(0);
    }

    self->field_00c8 = 0;
    self->field_00cc = -1;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 1;
    self->field_00d9 = 1;
    self->field_00da = 1;

    uint32_t childCount = CSF_AT(uint32_t, self, 0xE0);
    if (childCount < 0xA)
        GrowChildStorage(&self->field_00dc, nullptr, 10 - childCount);

    self->field_00f4 = 0;
    self->field_00f8 = 0;
    self->field_00fd = 0;
    self->field_00fc = 1;
    self->field_00fe = 1;
    self->field_0100 = 0.5f;
    self->field_0108 = -1;
    self->field_0104 = -1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00734B90, C_008670f0_InitializeChild, "auto");
