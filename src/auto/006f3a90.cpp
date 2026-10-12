#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, RpPrtStd_ReleaseParentState, 0x006C9F40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxParticleEmitters_FinalizeSettings, 0x006DE360, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, sub_6E04C0, 0x006E04C0, (void* self, void* edx, int a1, int a2));

CSF_GLOBAL(int, g_Value_8BB738, 0x008BB738);

static void __fastcall RpPrtStd_ResetAtomicState(C_00862748* self, void* /*edx*/)
{
    uint32_t savedField24 = self->field_0024;

    // First dword of the object is the vtable pointer (no declared __vftable member).
    *reinterpret_cast<void***>(self) = reinterpret_cast<void**>(CSF_VTBL(0x00862748));
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00862740)));
    self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00862724)));
    self->field_0020 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x008626F4)));

    if (savedField24 && g_Value_8BB738) {
        int* resultObject = reinterpret_cast<int*>(GetValue_8BB738());
        sub_6E04C0(resultObject, nullptr, static_cast<int>(self->field_002c), static_cast<int>(savedField24));
    }

    bool isNotEmitterOwned = (self->field_0034 == 0);
    self->field_0024 = 0;
    self->field_0038 = 0;
    self->field_0028 = 0;
    if (isNotEmitterOwned)
        self->field_0030 = 0;

    RpPrtStd_ResetBaseState(self, nullptr);

    if (self->field_0034) {
        S_006dcf90* emitterSettings = reinterpret_cast<S_006dcf90*>(self->field_0030);
        if (emitterSettings) {
            GfxParticleEmitters_FinalizeSettings(emitterSettings, nullptr);
            Mem_Free(emitterSettings);
            self->field_0030 = 0;
        }
        self->field_0030 = 0;
    }

    self->field_0020 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x008621DC)));
    RpPrtStd_ReleaseParentState(self, nullptr);
}

HOOK(0x006F3A90, RpPrtStd_ResetAtomicState, "auto");
