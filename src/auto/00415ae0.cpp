#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseSettingsSnapshot, 0x00410DA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CopySettingsSnapshot, 0x00410F50, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, InitializeScriptBase, 0x00411500, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_0082dfd0_Initialize(C_0082dfd0* self, void* /*edx*/)
{
    alignas(4) uint8_t settings[0x22C];

    InitializeScriptBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x0082DFD0);

    self->field_0090 = 0.0f;
    self->field_0094 = 0.0f;
    self->field_0098 = 0.0f;
    self->field_009c = 0.0f;
    self->field_00a0 = 0.0f;
    self->field_00a4 = 0.0f;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 1065353216;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00d0 = 1;
    self->field_011c = -1;
    self->field_0120 = -1;
    self->field_0118 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0124 = 0;
    self->field_0128 = 0;
    self->field_012c = 0;
    self->field_0140 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0144 = -1;
    self->field_0148 = -1;
    self->field_014c = 0;
    self->field_0150 = 0;
    self->field_0154 = 0;
    self->field_0158 = 10;
    self->field_015c = 0;
    self->field_0160 = 0;
    self->field_0164 = 0;

    self->field_0084 = 0.0f;
    self->field_0088 = 0;
    self->field_0089 = 0;
    self->field_008a = 0;
    self->field_008b = 0;
    self->field_008c = 0;
    self->field_008d = 1;
    self->field_008e = 0;
    self->field_00d4 = 0.0f;
    self->field_00d8 = 0.0f;
    self->field_00dc = 0;
    self->field_00e0 = 0.0f;
    self->field_00e4 = 0.0f;
    self->field_00e8 = 0.0f;
    self->field_00ec = 0.0f;
    self->field_00f0 = 0.0f;
    self->field_00f4 = 0.0f;
    self->field_00f8 = 0.0f;
    self->field_00fc = 0.0f;
    self->field_010c = 0;
    self->field_010d = 0;
    self->field_010e = 0;
    self->field_010f = 0;
    self->field_0110 = 1.0f;

    const int globalSettings = GetGlobalValue_753A30();
    CopySettingsSnapshot(settings, nullptr, globalSettings + 8);

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        self->field_0100 = CSF_AT(float, settings, 0x180) / CSF_AT(float, settings, 0x18C);
    else
        self->field_0100 = CSF_AT(float, settings, 0x154) / CSF_AT(float, settings, 0x160);

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        self->field_0104 = CSF_AT(float, settings, 0x184) / CSF_AT(float, settings, 0x190);
    else
        self->field_0104 = CSF_AT(float, settings, 0x158) / CSF_AT(float, settings, 0x164);

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        self->field_0108 = CSF_AT(float, settings, 0x188) / CSF_AT(float, settings, 0x194);
    else
        self->field_0108 = CSF_AT(float, settings, 0x15C) / CSF_AT(float, settings, 0x168);

    self->field_0114 = 0;
    self->field_0115 = 0;
    self->field_0130 = 0.0f;
    self->field_0134 = -1.0e26f;
    self->field_013c = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x948);
    self->field_0144 = -1;
    self->field_0148 = -1;

    const uint32_t recordCount = self->field_0150;
    if (recordCount < 0x10)
        MissionScript_GrowEventRecords(
            reinterpret_cast<uint8_t*>(self) + 0x14C, nullptr,
            static_cast<int>(16 - recordCount));

    ReleaseSettingsSnapshot(settings, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x00415AE0, C_0082dfd0_Initialize, "auto");
