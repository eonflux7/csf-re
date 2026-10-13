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
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, WeaponFirearms_SetEntry, 0x00476170, (void* self, void* , void* entry));

// Maps the stored scale mode to the float bit pattern the original pushes:
// 0 -> 0.75f, 1 -> 1.0f, 2 -> 1.5f, anything else -> 1.0f.
static uint32_t OptionsFile_ScaleModeToFloatBits(uint32_t mode)
{
    if (mode == 0)
        return 0x3F400000u; // 0.75f
    if (mode == 2)
        return 0x3FC00000u; // 1.5f
    return 0x3F800000u;     // 1.0f
}

static void __fastcall OptionsFile_ApplyScaleSetting(S_0050aac0* self, void* /*edx*/)
{
    uint32_t scaleBits = OptionsFile_ScaleModeToFloatBits(self->field_0028);

    C_00860130* globalObj = reinterpret_cast<C_00860130*>(GetGlobalInt());
    C_0085e7b8* accessor = reinterpret_cast<C_0085e7b8*>(
        GlobalInt_ResolveAccessor(globalObj, nullptr));
    void* provider = GetCachedObjectProvider(accessor, nullptr);

    WeaponFirearms_SetEntry(provider, nullptr,
        reinterpret_cast<void*>(static_cast<uintptr_t>(scaleBits)));
}

HOOK(0x50AB20, OptionsFile_ApplyScaleSetting, "auto");
