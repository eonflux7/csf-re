#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_ReleaseParentState, 0x006C9F40, (C_00860a30* self, void*));
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(void, __fastcall, GfxParticleEmitters_ReleaseEntry, 0x006E17B0, (void* self, void* , int entryIndex));
CSF_FUNC(void, __fastcall, ParticleEmitters_ReleaseEntry, 0x006E1860, (void* self, void* , int index));

static void __fastcall C_00862598_ReleaseElementState(C_00862598* self, void* /*edx*/)
{
    uint32_t gfxEmitterEntry = self->field_0024;

    self->vtbl = CSF_VTBL(0x00862598);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0086258C));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00862570));
    self->field_0020 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00862544));

    if (gfxEmitterEntry != static_cast<uint32_t>(-1)) {
        int emitterOwner = GetValue_8BB738();
        GfxParticleEmitters_ReleaseEntry(reinterpret_cast<void*>(static_cast<uintptr_t>(emitterOwner)),
                                         nullptr, static_cast<int>(gfxEmitterEntry));
        self->field_0024 = static_cast<uint32_t>(-1);
    }

    if (self->field_0028 != static_cast<uint32_t>(-1)) {
        uint32_t particleEntry = self->field_0028;
        int emitterOwner = GetValue_8BB738();
        ParticleEmitters_ReleaseEntry(reinterpret_cast<void*>(static_cast<uintptr_t>(emitterOwner)),
                                      nullptr, static_cast<int>(particleEntry));
        self->field_0028 = static_cast<uint32_t>(-1);
    }

    RpPrtStd_ResetBaseState(self, nullptr);
    CSF_AT(void*, self, 0x20) = CSF_VTBL(0x008621DC);
    C_00860a30_ReleaseParentState(self, nullptr);
}

HOOK(0x006F27A0, C_00862598_ReleaseElementState, "auto");
