#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResetBaseState, 0x0040FBF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseStateSnapshot, 0x00410DA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CopyStateSnapshot, 0x00410F50, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_0082dfd0_ResetState(C_0082dfd0* self, void* /*edx*/)
{
    alignas(4) std::uint8_t snapshot[0x22C];

    ResetBaseState(self, nullptr);

    self->field_0084 = 0.0f;
    self->field_0088 = 0;
    self->field_0089 = 0;
    self->field_008a = 0;
    self->field_008c = 0;
    self->field_008d = 1;

    self->field_0090 = g_ZeroVectorX;
    CSF_AT(float, self, 0x094) = g_ZeroVectorY;
    CSF_AT(float, self, 0x098) = g_ZeroVectorZ;
    self->field_009c = g_ZeroVectorX;
    CSF_AT(float, self, 0x0A0) = g_ZeroVectorY;
    CSF_AT(float, self, 0x0A4) = g_ZeroVectorZ;

    std::memcpy(&self->field_0024, &g_ZeroVectorX, sizeof(float));
    CSF_AT(float, self, 0x028) = g_ZeroVectorY;
    CSF_AT(float, self, 0x02C) = g_ZeroVectorZ;

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

    const int globalValue = GetGlobalValue_753A30();
    CopyStateSnapshot(snapshot, nullptr, globalValue + 8);

    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        self->field_0100 = CSF_AT(float, snapshot, 0x180) / CSF_AT(float, snapshot, 0x18C);
    else
        self->field_0100 = CSF_AT(float, snapshot, 0x154) / CSF_AT(float, snapshot, 0x160);

    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        self->field_0104 = CSF_AT(float, snapshot, 0x184) / CSF_AT(float, snapshot, 0x190);
    else
        self->field_0104 = CSF_AT(float, snapshot, 0x158) / CSF_AT(float, snapshot, 0x164);

    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        self->field_0108 = CSF_AT(float, snapshot, 0x188) / CSF_AT(float, snapshot, 0x194);
    else
        self->field_0108 = CSF_AT(float, snapshot, 0x15C) / CSF_AT(float, snapshot, 0x168);

    self->field_010c = 0;
    self->field_010d = 0;
    self->field_010e = 0;
    self->field_010f = 0;
    self->field_0110 = 1.0f;
    self->field_0114 = 0;
    self->field_0115 = 0;
    self->field_011c = -1;
    self->field_0120 = -1;

    std::memcpy(&self->field_0124, &g_ZeroVectorX, sizeof(float));
    CSF_AT(float, self, 0x128) = g_ZeroVectorY;
    CSF_AT(float, self, 0x12C) = g_ZeroVectorZ;
    self->field_0130 = 0.0f;

    return ReleaseStateSnapshot(snapshot, nullptr);
}

HOOK(0x004158F0, C_0082dfd0_ResetState, "auto");
