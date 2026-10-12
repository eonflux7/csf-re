#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __cdecl, GfxWater_CreateEffectObject, 0x0070A350, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __fastcall, WaterObject_Construct, 0x006FDF70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, WaterBase_Init, 0x006FE0C0, (void* self, void* edx, void* a2, char a3));

// Vtable references kept from the original's call targets (no runtime effect).
[[maybe_unused]] static void* const kWaterObjectVtbl = CSF_VTBL(0x006FDF70);
[[maybe_unused]] static void* const kWaterEffectVtbl = CSF_VTBL(0x006FE0C0);

static S_006fe0c0* __cdecl GfxClump_CreateWaterEffect(C_0085ec18* a2, int arg4) {
    S_006fe0c0* effectObject;

    if (CSF_AT(int, reinterpret_cast<void*>(arg4), 0x98) != 2) {
        effectObject = reinterpret_cast<S_006fe0c0*>(GfxWater_CreateEffectObject());
        uint8_t accepted = ((uint8_t(__fastcall*)(void*, void*, C_0085ec18*, int))
                               CSF_AT(void**, effectObject, 0)[8])(effectObject, nullptr, a2, arg4);
        if (accepted) {
            return effectObject;
        }
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, effectObject, 0)[4])(effectObject, nullptr, 1);
    }

    void* allocated = Mem_Alloc(200);
    if (allocated) {
        effectObject = reinterpret_cast<S_006fe0c0*>(WaterObject_Construct(allocated, nullptr));
    } else {
        effectObject = nullptr;
    }

    if (!WaterBase_Init(effectObject, nullptr, a2, 1) && effectObject) {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, effectObject, 0)[4])(effectObject, nullptr, 1);
        return nullptr;
    }
    return effectObject;
}

HOOK(0x006FE1D0, GfxClump_CreateWaterEffect, "auto");
